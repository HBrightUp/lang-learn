use std::{
    fs::{
        self,DirEntry
    },
    path::{
        Path,
    },
    io::{
        self,
    },
};


pub fn stdfs() {

    // get absolute path
    let _path = fs::canonicalize("./src").unwrap();

    let _ = fs::copy("./test.md", "./test_copy.md");
    let _ = fs::create_dir("./samwell");
    let _ = fs::create_dir_all("./some/dir/new");
    assert_eq!(fs::exists("./some").unwrap(), true);

    //get metadata info of file
    let metadata = fs::metadata("./test.md").unwrap();
    let content: Vec<u8> = fs::read("./test.md").unwrap();
    assert_eq!(content.len(), metadata.len() as usize);

    // traversal dircetory
    let d1 = String::from("./some");
    let path = Path::new(&d1);
    let _ = visit_dirs(path, &deal_file);
}



// 遍历指定文件夹下面的所有文件
fn visit_dirs(dir: &Path, cb: &dyn Fn(&DirEntry)) -> io::Result<()> {
    if dir.is_dir() {
        for entry in fs::read_dir(dir)? {
            let entry = entry?;
            let path = entry.path();
            if path.is_dir() {
                visit_dirs(&path, cb)?;
            } else {
                cb(&entry);
            }
        }
    }
    Ok(())
}

//定义回调函数，处理文件
fn deal_file(entry: &DirEntry) {
    //get file information on here,like file_name / file_type / path and so on.

    //read  and write file
    let add = "rust".to_string();
    let mut s1 = fs::read_to_string(&entry.path()).unwrap();
    let len_before = s1.len();
    s1.push_str(add.as_str());
    let _ = fs::write(&entry.path(), s1);
    let s2 = fs::read_to_string(&entry.path()).unwrap();
    
    assert_eq!(len_before + add.len(), s2.len());
}