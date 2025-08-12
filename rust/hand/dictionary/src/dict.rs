use std::sync::Arc;
use std::sync::{RwLock, RwLockReadGuard, RwLockWriteGuard};
use std::collections::HashMap;


pub struct Dictionary{

    // record using times of words.
    dict: Arc<RwLock<HashMap<String, usize>>>,

    // Specify the file suffix, non thread-safe
    file_suffix: Vec<String>,
}

impl Dictionary {
    pub fn new() -> Self {
        Self{
            dict: Arc::new(RwLock::new(HashMap::new())),
            file_suffix: vec![],
        }
    }

    pub fn set_file_suffix(&mut self, suffix: &str) {
        self.file_suffix.push(suffix.to_string());
    }

    pub fn get_file_suffix(&self) -> &Vec<String> {
        self.file_suffix.as_ref()
    }

    pub fn get_dict_as_ref(&self) -> RwLockReadGuard<'_, HashMap<String, usize>> {
        self.dict.read().unwrap()
    }

    pub fn get_dict_as_mut(&self) -> RwLockWriteGuard<'_, HashMap<String, usize>> {
        self.dict.write().unwrap()
    }
}


#[cfg(test)]
mod test {
    use super::*;

    #[test] 
    fn init() {
        let mut d = Dictionary::new();

        d.add_file_suffix("rs");
        d.add_file_suffix("md");

        let suffix = d.get_file_suffix();

        assert_eq!(suffix, &vec!["rs".to_string(), "md".to_string()]);
    }
}