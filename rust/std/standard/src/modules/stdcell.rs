use std::{
    cell::{
        RefCell,
    },
    collections::{
        HashMap,
    },
    rc::{
        Rc,
    },
};



fn load_configs(config: &Rc<RefCell<HashMap<String, String>>>) {
    let mut map = config.borrow_mut();
    map.insert("host".to_owned(), "0.0.0.0".to_owned());
    map.insert("port".to_owned(), "8080".to_owned());
    map.insert("db_url".to_owned(), "mysql://localhost:3306".to_owned());
    map.insert("db_username".to_owned(), "root".to_owned());
}

struct HttpServer {
    config: Rc<RefCell<HashMap<String, String>>>,
}

impl HttpServer {
    fn fill_defaults(config: &Rc<RefCell<HashMap<String, String>>>) {
        let mut map = config.borrow_mut();
        if !map.contains_key("port") {
            map.insert("port".to_owned(), "8080".to_owned());
        }
    }

    fn new(config: Rc<RefCell<HashMap<String, String>>>) -> Self {
        Self::fill_defaults(&config);
        Self {
            config
        }
    }

    fn listen(&self) {
        let map = self.config.borrow();

        assert_eq!(map.get("host").unwrap(), "0.0.0.0");
        assert_eq!(map.get("port"), Some(&"8080".to_string()));
    }
}
struct Database {
    config: Rc<RefCell<HashMap<String, String>>>,
}
impl Database {
    fn fill_defaults(config: &Rc<RefCell<HashMap<String, String>>>) {
        //通过使用RefCell修改config中的数据
        let mut map = config.borrow_mut();
        if !map.contains_key("db_password") {
            map.insert("db_password".to_owned(), "admin".to_owned());
        }
    }

    fn new(config: Rc<RefCell<HashMap<String, String>>>) -> Self {
        Self::fill_defaults(&config);
        Self{ config }
    }

    fn connect(&self) {
        let map = self.config.borrow();

        assert_eq!(map.get("db_url").unwrap(), "mysql://localhost:3306");
        assert_eq!(map.get("db_username").unwrap(), "root");
        assert_eq!(map.get("db_password").unwrap(), "admin");
    }
}

pub fn stdcell() {

    //使用 RefCell 共享多模块配置信息
    let config =Rc::new(RefCell::new(HashMap::new()));
    load_configs(&config);

    let db = Database::new(config.clone());
    let http = HttpServer::new(config.clone());

    db.connect();
    http.listen();
}
