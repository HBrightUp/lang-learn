
use env_logger;
use std::env;
use log:: { info, trace, error};
use std::fs;
use std::collections:: { HashSet, BTreeMap };
use std::str::FromStr;
use std::num::ParseIntError;
use std::hash::{ Hash, Hasher };
use std::fmt::{self, Formatter, Display};

static  PACKAGE_INFO_ITEMS: usize = 5;

#[derive( Debug, Hash, Clone, PartialEq, Eq)]
enum Priority {
    Required,
    Advanced,
    Basic,
    Obtained,
    Interesting,
    Unknown,
    ViewedNoExample,
    Unnecessary,
    Empty,
}

impl FromStr for Priority {
    type Err = ParseIntError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let e = s.parse::<u8>().unwrap();
        let pri = match e {
            10_u8 => Priority::Required,
            9_u8 => Priority::Advanced,
            8_u8 => Priority::Basic,
            7_u8 => Priority::Obtained,
            6_u8 => Priority::Interesting,
            5_u8 => Priority::Unknown,
            4_u8 => Priority::ViewedNoExample,
            1_u8 => Priority::Unnecessary,
            _ => todo!(),
        };

        Ok(pri)
    }
}

#[derive(Eq, PartialEq)]
enum PackAttr{
    Name,
    Discrible,
    TotalUsed,
    RecentUsed,
    Priority,
    Empty,
}

#[derive( Debug, Clone)]
struct PackInfo{
    name: String,
    discrible: String,
    total_used: usize,
    recent_used: usize,
    priority: Priority,
}

impl Hash for  PackInfo {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.name.hash(state);
    }
}

impl PackInfo {
    fn new() ->Self {
        PackInfo {
            name: String::new(),
            discrible: String::new(),
            total_used: 0,
            recent_used: 0,
            priority: Priority::Unknown,
        }
    }
}

impl PartialEq for PackInfo {
    fn eq(&self, other: &Self) -> bool {

        //Warning: Because == was written as >=, the data storage in hashset was messed up.
        // When using hashset insert, some values ​​were not stored, but false was returned during insert.
        // Moreover, the key value conflicts generated each time the program was run were different, 
        //which was quite confusing. The cause has not been found yet.
        //self.name >= other.name
        self.name == other.name
    }
}

impl Eq for PackInfo {
    
}


fn main() {
    init();
    info!("Hello, hml");
    info!("Current workspace director: {:?}", env::current_dir().unwrap().display());

    let path = "./solana.pack";
    let content = fs::read_to_string(path).unwrap();

    let mut packattr = PackAttr::Name;
    let mut h: HashSet<PackInfo> = HashSet::new();
    let mut p = PackInfo::new();

    for line in content.lines() {

        match  packattr {
            PackAttr::Name => {
                p.name = line.to_string().clone();
                packattr = PackAttr::Discrible;
            }
            PackAttr::Discrible => {
                p.discrible = line.to_string().clone();
                packattr = PackAttr::TotalUsed;
            }
            PackAttr::TotalUsed => {
                p.total_used = line.trim().parse().unwrap();
                packattr = PackAttr::RecentUsed;
            }
            PackAttr::RecentUsed => {
                
                p.recent_used = line.parse().unwrap();
                packattr = PackAttr::Priority;
            }
            PackAttr::Priority => {
                p.priority = line.parse::<Priority>().unwrap();
                let is_existed = h.insert(p.clone());
                if !is_existed {
                    error!("Insert failed! The parckinfo is :{:?}", &p);
                }
                packattr = PackAttr::Empty;
            }
            PackAttr::Empty =>{
                assert!(line.to_string().is_empty());
                packattr = PackAttr::Name;
            }
        }
    }
    
    //assert_eq!(h.len(), 203);

    statistics_pack(&h);
    trace!("HashSet len: {:?}", h.len());
    info!("Finish data analyze !");

    
}

fn init() {
    unsafe { env::set_var("RUST_LOG", "info"); }
    env_logger::init();
}

fn statistics_pack(pack: &HashSet<PackInfo>) {
    
    
    info!("start statistics packinfo ... ");
    let mut required_btmap : BTreeMap<usize, String> = BTreeMap::new();
    let mut advanced_btmap : BTreeMap<usize, String> = BTreeMap::new();
    let mut basic_btmap: BTreeMap<usize, String> = BTreeMap::new();
    let mut obtained_btmap: BTreeMap<usize, String> = BTreeMap::new();
    let mut interesting_btmap: BTreeMap<usize, String> = BTreeMap::new();
    let mut unknown_btmap: BTreeMap<usize, String> = BTreeMap::new();
    let mut noexaple_btmap: BTreeMap<usize, String> = BTreeMap::new();
    let mut unnecessary_btmap: BTreeMap<usize, String> = BTreeMap::new();

    for data in pack {
        match data.priority {
            Priority::Required => {
                let weight = get_packinfo_weight(data);
                required_btmap.insert(weight, data.name.clone());
            },
            Priority::Advanced => {
                let weight = get_packinfo_weight(data);
                advanced_btmap.insert(weight, data.name.clone());
            },
            Priority::Basic => {
                let weight = get_packinfo_weight(data);
                basic_btmap.insert(weight, data.name.clone());
            },
            Priority::Obtained => {
                let weight = get_packinfo_weight(data);
                obtained_btmap.insert(weight, data.name.clone());
            },
            Priority::Interesting => {
                let weight = get_packinfo_weight(data);
                interesting_btmap.insert(weight, data.name.clone());
            },
            Priority::Unknown => {
                let weight = get_packinfo_weight(data);
                unknown_btmap.insert(weight, data.name.clone());
            },
            Priority::ViewedNoExample => {
                let weight = get_packinfo_weight(data);
                noexaple_btmap.insert(weight, data.name.clone());
            },
            Priority::Unnecessary => {
                let weight = get_packinfo_weight(data);
                unnecessary_btmap.insert(weight, data.name.clone());
            },
            Priority::Empty => {
               todo!();
            }
        }
    }

    println!("Finish statistics, the result as follows: ");
    let mut print = String::new();

    print_name(&required_btmap, &mut print, "Require");
    print_name(&advanced_btmap, &mut print, "Advanced");
    print_name(&basic_btmap, &mut print, "Basic");
    print_name(&obtained_btmap, &mut print, "Obtained");
    print_name(&interesting_btmap, &mut print, "Interesting");
    print_name(&unknown_btmap, &mut print, "Unknown");
    print_name(&noexaple_btmap, &mut print, "Noexample");
    print_name(&unnecessary_btmap, &mut print, "Unnecessary");

    let len = required_btmap.len() + advanced_btmap.len() + basic_btmap.len() + obtained_btmap.len() +interesting_btmap.len() + 
        unknown_btmap.len() + noexaple_btmap.len() + unnecessary_btmap.len();
    
    assert_eq!(len, pack.len());

    println!("{print}");
    std::fs::write("./statistics_pack", print);

}

fn print_name(btmap: &BTreeMap<usize, String>, print: &mut String, name: &str) {

    print.push_str(format!("******************** {name}:{} ********************\n", btmap.len()).as_str());
    for (k,v) in btmap {
        print.push_str(&format!("{k}: {v}").as_str());
        print.push('\n');
    }
}

fn get_packinfo_weight(pack: &PackInfo) -> usize {
    pack.total_used * 60 / 100 + pack.recent_used * 40 / 100
}