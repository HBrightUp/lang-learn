use std::{
    env::{
        self,
    },
    collections::{
        HashMap,
    },
};


pub fn stdenv() {

    //args list
    for _argument in env::args() {
        //println!("{argument}");
    }

    let _cur_dir = env::current_dir().unwrap().display().to_string();
    let _home_dir = env::home_dir().unwrap().display().to_string();
    let _current_exe_dir = env::current_exe().unwrap().display().to_string();
    assert_eq!(env::temp_dir().display().to_string(), "/tmp".to_string());

    //set and unset env variable
    let key = "KEY";
    unsafe {
        env::set_var(key, "VALUE");
    }    
    assert_eq!(env::var(key), Ok("VALUE".to_string()));
    unsafe {
        env::remove_var(key);
    }
    assert!(env::var(key).is_err());

    // get all env variable
    let mut map_env: HashMap<String, String> =  HashMap::new();
    for (key, value) in std::env::vars() {
        map_env.insert(key, value);
    }
    
    assert_eq!(map_env["SHELL"], "/bin/bash");

}

