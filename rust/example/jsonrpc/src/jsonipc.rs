

use {log::{ warn, info},
    jsonrpc_core::{BoxFuture, ErrorCode, MetaIoHandler, Metadata, Result},
    jsonrpc_derive::rpc,
    jsonrpc_ipc_server::{
        tokio::sync::oneshot::channel as oneshot_channel, RequestContext, ServerBuilder,
    },
    jsonrpc_core_client::{transports::ipc, RpcError},
    std::{
        path::Path,
        thread::{self, Builder},
        sync::{Arc, RwLock},
    },
};
use crate::exit::Exit;


#[rpc]
pub trait AdminRpc {
    type Metadata;

    #[rpc(meta, name = "exit")]
    fn exit(&self, meta: Self::Metadata) -> Result<()>;

    #[rpc(meta, name = "hello")]
    fn hello(&self, meta: Self::Metadata) -> Result<()>;
}

pub struct AdminRpcImpl;
impl AdminRpc for AdminRpcImpl {
    type Metadata = AdminRpcRequestMetadata;

    fn hello(&self, meta: Self::Metadata) -> Result<()> {
        info!("Hello, AdminRpcImpl received your message");
        Ok(())
    }

    fn exit(&self, meta: Self::Metadata) -> Result<()> {
        Ok(())
    }
}

#[derive(Clone, Default)]
pub struct AdminRpcRequestMetadata {

    pub validator_exit: Arc<RwLock<Exit>>,
    pub post_init: Arc<RwLock<Option<AdminRpcRequestMetadataPostInit>>>,

}

impl Metadata for AdminRpcRequestMetadata {}

impl AdminRpcRequestMetadata {
    fn with_post_init<F, R>(&self, func: F) -> Result<R>
    where
        F: FnOnce(&AdminRpcRequestMetadataPostInit) -> Result<R>,
    {
        if let Some(post_init) = self.post_init.read().unwrap().as_ref() {
            func(post_init)
        } else {
            Err(jsonrpc_core::error::Error::invalid_params(
                "Retry once validator start up is complete",
            ))
        }
    }
}

#[derive(Clone)]
pub struct AdminRpcRequestMetadataPostInit {
  
}


pub fn run() {
    start_ipc_server();

    call_ipc_server();
}

fn start_ipc_server() {
    let admin_rpc_path: &Path = Path::new("./admin.rpc");
    let mut metadata = AdminRpcRequestMetadata::default();

    let event_loop = tokio::runtime::Builder::new_multi_thread()
        .thread_name("solAdminRpcEl")
        .worker_threads(3) // Three still seems like a lot, and better than the default of available core count
        .enable_all()
        .build()
        .unwrap();

    println!("current_dir: {:?}", std::env::current_dir().unwrap().display());

    Builder::new()
        .name("solAdminRpc".to_string())
        .spawn(move || {
            let mut io = MetaIoHandler::default();
            io.extend_with(AdminRpcImpl.to_delegate());

            let validator_exit = metadata.validator_exit.clone();
            let server = ServerBuilder::with_meta_extractor(io, move |_req: &RequestContext| {
                metadata.clone()
            })
            .event_loop_executor(event_loop.handle().clone())
            .start(&format!("{}", admin_rpc_path.display()));
         
            match server {
                Err(err) => {
                    warn!("Unable to start admin rpc service: {:?}", err);
                }
                Ok(server) => {
                    info!("started admin rpc service!");
                    let close_handle = server.close_handle();
                    validator_exit
                        .write()
                        .unwrap()
                        .register_exit(Box::new(move || {
                            close_handle.close();
                        }));

                    // Here will be block util the main thread end.
                    server.wait();
                }
            }
        })
        .unwrap();
}

fn call_ipc_server() {

    let ledger_path = Path::new("./");
    let admin_client = connect(&ledger_path);
    runtime()
        .block_on( async {
            admin_client.await?.hello().await
        })
        .unwrap_or_else(|err| {
            println!("Failed to call hello.");
            return ;
        });
    println!("Successfully say hello.");
}

async fn connect(ledger_path: &Path) -> std::result::Result<gen_client::Client, RpcError> {

    let admin_rpc_path = ledger_path.join("admin.rpc");
    if !admin_rpc_path.exists() {
        Err(RpcError::Client(format!(
            "{} does not exist",
            admin_rpc_path.display()
        )))
    } else {
        ipc::connect::<_, gen_client::Client>(&format!("{}", admin_rpc_path.display())).await
    }
}

pub fn runtime() -> jsonrpc_server_utils::tokio::runtime::Runtime {
    jsonrpc_server_utils::tokio::runtime::Runtime::new().expect("new tokio runtime")
}