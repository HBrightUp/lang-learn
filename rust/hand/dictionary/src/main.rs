use std::{
    env,io,
    path::Path,
    time::Instant,
    ops::Sub,
    fs::{
        self,DirEntry,
    },
}; 

use dictionary::dict::Dictionary;

#[derive(Debug)]
enum ParseError{
    IndexOverflow, 
    NoneString,  
    GetPathFailed,
}

type ArgResult = Result<String, ParseError>;
type ParseResult = Result<(usize, usize), ParseError>;


fn main() {
    println!("Hello, hml~");

    let dir = match get_path(){
        Ok(p) => p,
        Err(e) => {
            println!("Error: {:?}", e);
            return ;
        }
    };
    
    let mut d = Dictionary::new();
    init(&mut d);

    let p = Path::new(&dir);
    let start_instant = Instant::now();
    let _ = visit_dirs(p, &deal_file, &d);
    println!("Speed time: {:?}", Instant::now().sub(start_instant));

    save_Record(&d);
}

fn init(d: &mut Dictionary) {
    d.set_file_suffix("rs");
    d.set_file_suffix("md");
}

fn get_path() ->  ArgResult {
    
    let mut args = vec![];
    for arg in env::args() {
        args.push(arg);
    }

    if args.len() < 2 {
        println!("Please running with a directory path, eg: cargo run ./test");
        return Err(ParseError::GetPathFailed);
    }

    let dir = args[1].to_string();
    println!("file path is: {}", dir);

    Ok(dir)
}


fn save_Record(d: &Dictionary) {
    
    let dict = d.get_dict_as_ref();
    let mut s = String::new();

    let mut times: usize = 0;
    let mut  most_word = String::new();
    let mut total_words: usize = 0;

    //Method 1: non-order
    for (k, v) in dict.iter() {
        if *v > times {
            times = *v;
            most_word = k.clone();
        }
        total_words += *v;

       s.push_str(&format!("{}: {} \n", k, v));
    }

    println!("Amount of words: {}", total_words);
    println!("The most common word is : {}  {}", most_word, times);
    let save_path = "./Record.md";
    let _ = fs::write(save_path, s);

}

// Traverse folders
fn visit_dirs(dir: &Path, cb: &dyn Fn(&DirEntry, &Dictionary), d: &Dictionary ) -> io::Result<()> {
    if dir.is_dir() {
        for entry in fs::read_dir(dir)? {
            let entry = entry?;
            let path = entry.path();

            if path.is_dir() {
                visit_dirs(&path, cb, d)?;
            } else {
                if is_need_deal(&entry, d) {
                    cb(&entry, d);
                }

            }
        }
    }
    Ok(())
}

fn is_need_deal(entry: &DirEntry, d: &Dictionary) -> bool {
        
    let  metadata = entry.metadata().unwrap();
    let mut is_deal = true;
    
    // Only process defined file suffixes
    if let Some(ext) = Path::new(&entry.path()).extension() {

        let filter = d.get_file_suffix();
        let mut is_find = false;

        for e in filter {
            if  *ext == **e {
                is_find = true;
                break;
            }
        }

        is_deal = is_find;

    } else {
        is_deal  = false;
    }
    
    if !metadata.is_file() {
        is_deal  = false;
    }

    //empty file
    if metadata.len() == 0 {
        is_deal  = false;
    }

    is_deal
}

fn deal_file(entry: &DirEntry, d: &Dictionary) {

    println!("deal file: {}", entry.path().display());

    let content = fs::read(entry.path()).unwrap();
    let len = content.len();
    let mut pos = 0;

    while pos < len {
        let (start, end) = match get_letter_postion(&content, pos, len) {
            Ok(range) => range,
            Err(_e) => {
                break;
            }
        };

        let key =  &content.as_slice()[start..end];
        let key_string: String = String::from_utf8_lossy(key).to_string();
        let mut dict = d.get_dict_as_mut();

        dict.entry(key_string).and_modify(|counter| *counter += 1).or_insert(1);
        pos = end;
    }
}

fn get_letter_postion(content: &Vec<u8>, start: usize, len: usize) -> ParseResult {

    if start >= len {
         return Err(ParseError::IndexOverflow);
    }

    let mut indexes: (usize, usize) = (0, 0);
    let mut  is_indexes_0_init = false;

    for i in start..len {
        if content[i].is_ascii_alphabetic() {
            if !is_indexes_0_init {
                indexes.0 = i;
                is_indexes_0_init = true;
            } 

        } else {
            if !is_indexes_0_init  {
                continue;
            } else {
                indexes.1 = i;
                break;
            }
        }
    }

    // Special conditions: No characters found at the end of the file.
    if indexes.0 == 0 && indexes.1 == 0 {
        return Err(ParseError::NoneString);
    }

    Ok(indexes)
}

