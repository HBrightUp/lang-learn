use serde::{ Serialize, Deserialize, Serializer, Deserializer };
use log::info;
use serde_json;
use std::collections::HashMap;

//serde supported formats: JSON, Bincode, MessagePack, CBOR, YAML, TOML, RON, BSON, JSON5, URL


// serde struct
#[derive(Serialize, Deserialize, Debug)]
struct AnimalStruct {
    name: String,
    age: u32,
}

// serde enum
#[derive(Serialize, Deserialize, Debug)]
enum AnimalEnum {
    Dog { name: String, age: u32 },
    Cat { name: String, age: u32 },
}

// // serde option
#[derive(Serialize, Deserialize, Debug)]
struct AnimalOption {
    name: Option<String>,
    age: u32,
}

// serde vec
#[derive(Serialize, Deserialize, Debug)]
struct Zoo {
    animals: Vec<AnimalStruct>,
}

// serde costom trait
#[derive(Serialize, Deserialize, Debug)]
struct ZooHashmap {
    animals: HashMap<String, AnimalStruct>,
}

// // serde struct
#[derive(Serialize, Deserialize, Debug)]
struct AnimalCustom {
    #[serde(serialize_with = "serialize_name", deserialize_with = "deserialize_name")]
    name: String,
    age: u32,
}



pub fn enter() {
    serde_struct();
    serde_enum();
    serde_option();
    serde_vec();
    serde_hashmap();
    serde_costom();

}
fn serde_costom() {
    let animal = AnimalCustom {
        name: "rust".to_string(),
        age: 8,
    };

    let json = serde_json::to_string(&animal).unwrap();
    println!("{}", json);   //{"name":"RUST","age":8}

    let json = r#"{"name":"RUST","age": 20}"#;
    let costom : AnimalCustom = serde_json::from_str(json).unwrap();

    println!("{:?}", costom);  // AnimalCustom { name: "rust", age: 20 }
}

fn serde_hashmap() {

    let mut animals = HashMap::new();
    animals.insert("Tom".to_owned(), AnimalStruct { name: "Tom".to_owned(), age: 3 });
    animals.insert("Jerry".to_owned(), AnimalStruct { name: "Jerry".to_owned(), age: 2 });

    let zoo = ZooHashmap { animals };
    let json = serde_json::to_string(&zoo).unwrap();

    // {"animals":{"Jerry":{"name":"Jerry","age":2},"Tom":{"name":"Tom","age":3}}}
    println!("{}", json); 

    let json = r#"{"animals":{"Jerry":{"name":"Jerry","age":2},"Tom":{"name":"Tom","age":3}}}"#;
    let zoo: ZooHashmap = serde_json::from_str(json).unwrap();

    // ZooHashmap { animals: {"Jerry": AnimalStruct { name: "Jerry", age: 2 }, "Tom": AnimalStruct { name: "Tom", age: 3 }} }
    println!("{:?}", zoo); 
}

fn serde_vec() {

    let zoo = Zoo { animals: vec![
        AnimalStruct { name: "Tom".to_owned(), age: 3 },
        AnimalStruct { name: "Jerry".to_owned(), age: 2 },
    ] };

    let json = serde_json::to_string(&zoo).unwrap();
    println!("{}", json); // {"animals":[{"name":"Tom","age":3},{"name":"Jerry","age":2}]}
    
    let json = r#"{"animals":[{"name":"Tom","age":3},{"name":"Jerry","age":2}]}"#;
    let zoo: Zoo = serde_json::from_str(json).unwrap();

    //Zoo { animals: [AnimalStruct { name: "Tom", age: 3 }, AnimalStruct { name: "Jerry", age: 2 }] }
    println!("{:?}", zoo); 
    
}


fn serde_option() {

    let animal = AnimalOption { name: Some("Tom".to_owned()), age: 3 };
    let json = serde_json::to_string(&animal).unwrap();
    println!("{}", json); // {"name":"Tom","age":3}

    let animal = AnimalOption { name: None, age: 4 };
    let json = serde_json::to_string(&animal).unwrap();
    println!("{}", json); // {"name":null,"age":4}

    let json = r#"{"age":3}"#;
    let animal: AnimalOption = serde_json::from_str(json).unwrap();
    println!("{:?}", animal); // AnimalOption { name: None, age: 3 }

}

fn serde_enum() {
    let dog = AnimalEnum::Dog { name: "Tom".to_owned(), age: 5 };
    let json_dog = serde_json::to_string(&dog).unwrap();

    //{"Dog":{"name":"Tom","age":5}}
    info!("{}", json_dog); 

   
    let json_cat = r#"{"Cat":{"name":"Tom","age":8}}"#;
    let cat: AnimalEnum = serde_json::from_str(json_cat).unwrap();

    // Cat { name: "Tom", age: 8 }
    info!("{:?}", cat); // Dog 
}


fn serde_struct() {
    let a1 = AnimalStruct {
        name: "Tom".to_owned(),
        age: 3,
    };

    let j1 = serde_json::to_string(&a1).unwrap();

    // {"name":"Tom","age":3}
    info!("{}", j1); 
    
    let j2 = r#"{"name":"Tom","age":10}"#;
    let a2: AnimalStruct = serde_json::from_str(j2).unwrap();

    // AnimalStruct { name: "Tom", age: 10 }
    info!("{:?}", a2); 

}

fn serialize_name<S>(name: &String, serializer: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    serializer.serialize_str(&name.to_uppercase())
}

fn deserialize_name<'de, D>(deserializer: D) -> Result<String, D::Error>
where
    D: Deserializer<'de>,
{
    let name = String::deserialize(deserializer)?;
    Ok(name.to_lowercase())
}

