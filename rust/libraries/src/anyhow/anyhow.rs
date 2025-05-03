use anyhow::Result;
use std::net::SocketAddr;

pub fn enter() {
    call_read_file_error();
    call_parse_addr_error();
    call_parse_error();
}




fn call_read_file_error() {
    let _config = match read_file_error() {
        Ok(cfg) => cfg,
        Err(e) => {
            println!("{e}");  // No such file or directory (os error 2)
            return ;
        }
    };
    
}

fn read_file_error() -> Result<String> {
    let config = std::fs::read_to_string("./not_exist.toml")?;
    Ok(config) 
}

fn call_parse_addr_error() {
    let _addr = match parse_addr_error() {
        Ok(addr) => addr,
        Err(e) => {
            println!("{e}");        // invalid socket address syntax
            return ;
        }
    };
    
}

fn parse_addr_error() -> Result<SocketAddr> {
    let addr = "127.0.0..1:8000".to_string();
    let addr = addr.parse::<SocketAddr>()?;

    Ok(addr)
}   

fn call_parse_error() {
    let _num = match parse_error() {
        Ok(n) => n,
        Err(e) => {
            println!("{e}");        // invalid digit found in string
            return ;
        }
    };
    
}

fn parse_error() -> Result<u64> {
    let data = "23tt";
    let num = data.parse::<u64>()?;     

    Ok(num)
}


