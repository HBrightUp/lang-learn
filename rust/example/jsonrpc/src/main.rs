use log::{
    info,
};
use std::env::set_var;
use std::ops::RangeInclusive;
use clap::{
    Parser, ValueEnum, Subcommand,
};

//mod json_ipc;
use jsonrpc::jsonipc;

const PORT_RANGE: RangeInclusive<usize> = 8000..=9999;

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, ValueEnum)]
enum Mode {
    Fast,
    Slow
}

#[derive( Subcommand, Clone)]
enum Commands{
    add { name: Option<String>},
    remove {
        names: Vec<String>,

        #[arg( short, long, default_value = "false")]
        force: bool,
    },
    start {
        protocal: String,
    },
}

#[derive(Parser)]
#[command( name = "hml" )]
#[command( author = "Bright" )]
#[command( version = "1.0" )]
#[command( about = "Rpc tutorial" )]
struct Cli {

    //Warning: non-required positional argument must be at front of positional argument, or some error:
    // Found non-required positional argument with a lower index than a required positional argument
    #[arg( value_parser = port_in_range)]
    port: Option<u16>,

    #[arg( short, long )]
    method: Option<String>,

    name: Option<String>,

    #[arg(short = 'p', long, default_value = "./test")]
    path: String,


    //Warn: By default, mode short will conflict with method's short.
    #[arg( value_enum, short = 'd', long )]
    mode: Option<Mode>,
    
  

    #[command(subcommand)]
    command: Commands,


}

pub fn port_in_range( s: &str) -> Result< u16, String> {
    let port = s.parse().map_err( |_| 
        format!(" '{s}' is not a port number.")
    )?;

    if PORT_RANGE.contains(&port) {
        Ok(port as u16)
    } else {
        Err(format!("Port not in range {}-{}", PORT_RANGE.start(), PORT_RANGE.end()))
    }
}



fn main() {
    init();
    
    let cli = Cli::parse();
    let path = cli.path;
    let method = match cli.method {
        Some(m) => m,
        None => "hello".to_owned(),
    };
    let name = match cli.name {
        Some(n) => n,
        None => "world".to_owned(),
    };

    //info!("method: {method}, name: {name}");

    match &cli.command {

        Commands::add { name } => {
            if name.is_none() {

            } else {

            }
        },
        Commands::remove{
            names,
            force,
        } => {

        },
        Commands::start{
            protocal,
        } => {
            if protocal == "ipc" {
                jsonipc::run();
            }


            
            
        }
    } 
    
    std::thread::sleep(std::time::Duration::from_secs(1200));
}




fn init() {
    set_var("RUST_LOG", "trace");
    env_logger::init();
}

