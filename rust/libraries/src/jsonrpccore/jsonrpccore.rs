
use jsonrpc_core::*;
use serde_derive::Deserialize;

use crate::jsonrpccore::middleware;


pub fn enter() {
    println!("jsonrpccore enter +++");

    middleware::test_middleware();
}









#[cfg(test)] 
mod test {

    use super::*;

    #[test]
    fn test_Params() {

        #[derive(Deserialize)]
        struct HelloParams {
            name: String,
        }

        let mut io = IoHandler::new();

        io.add_method("say_hello", |params: Params| async move {
            let parsed: HelloParams = params.parse().unwrap();
            Ok(Value::String(format!("hello, {}", parsed.name)))
        });

        let request = r#"{"jsonrpc": "2.0", "method": "say_hello", "params": { "name": "world" }, "id": 1}"#;
        let response = r#"{"jsonrpc":"2.0","result":"hello, world","id":1}"#;

        assert_eq!(io.handle_request_sync(request), Some(response.to_owned()));
    }

    #[test]
    fn test_meta() {

        #[derive(Clone, Default)]
        struct Meta(usize);
        impl Metadata for Meta {}


        let mut io = MetaIoHandler::default();

        io.add_method_with_meta("say_hello", |_params: Params, meta: Meta| async move {
            Ok(Value::String(format!("Hello World: {}", meta.0)))
        });

        let request = r#"{"jsonrpc": "2.0", "method": "say_hello", "params": [42, 23], "id": 1}"#;
        let response = r#"{"jsonrpc":"2.0","result":"Hello World: 5","id":1}"#;

        let headers = 5;
        assert_eq!(
            io.handle_request_sync(request, Meta(headers)),
            Some(response.to_owned())
        );
    }

    #[test]
    fn test_async() {
        futures_executor::block_on(async {
            let mut io = IoHandler::new();
    
            io.add_method("say_hello", |_: Params| async {
                Ok(Value::String("Hello World!".to_owned()))
            });
    
            let request = r#"{"jsonrpc": "2.0", "method": "say_hello", "params": [42, 23], "id": 1}"#;
            let response = r#"{"jsonrpc":"2.0","result":"Hello World!","id":1}"#;
    
            assert_eq!(io.handle_request(request).await, Some(response.to_owned()));
        });
    }
}