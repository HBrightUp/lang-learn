use hello_macro::{ HelloMacro, IntoHashMap } ;
use hello_macro_derive::{ HelloMacro, IntoHashMap, DeriveCustomModel, log_duration, constant_string,hash_mapify };
use std::collections::HashMap;

#[derive( HelloMacro, IntoHashMap)]
struct Pancakes{
    name: String,
    hobby: String,
}

#[derive(DeriveCustomModel)]
#[custom_model(model(
    name = "UserName",
    fields(first_name, last_name),
    extra_derives(IntoHashMap)
))]
#[custom_model(model(name = "UserInfo", fields(username, age), extra_derives(Debug)))]
pub struct User2 {
    username: String,
    first_name: String,
    last_name: String,
    age: u32,
}

#[log_duration]
#[must_use]
fn function_to_benchmark() -> u16 {
    let mut counter = 0;
    for _ in 0..u16::MAX {
        counter += 1;
    }

    counter
}

constant_string!("RUST");

fn main() {
    
    use_hello_macro();
    use_into_hashmap();
    use_costom_model();
    use_log_duration();
    use_constant_string();
    test_hash_mapify();
}

fn use_hello_macro() {
    Pancakes::hello_macro();
}

fn use_into_hashmap() {
    let p = Pancakes {
        name: "hml".to_string(),
        hobby: "flying".to_string(),
    };

    let hash_map = HashMap::<String, String>::from(p);
    dbg!(hash_map);
}

fn use_costom_model() {
    let user_name = UserName {
        first_name: "first_name".to_string(),
        last_name: "last_name".to_string(),
    };
    let hash_map = HashMap::<String, String>::from(user_name);

    dbg!(hash_map);

    let user_info = UserInfo {
        username: "username".to_string(),
        age: 27,
    };

    dbg!(user_info);
}

fn use_log_duration() {
    println!("{}", function_to_benchmark());
}

fn use_constant_string() {
    println!("constant string: {}", RUST);
}

fn test_hash_mapify() {
    let some_variable = "Some variable value";

    let hash_map = hash_mapify!(
        &str,
        "first_key" = "first_value",
        "second_variable" = some_variable,
        some_key = "value for variable key",
    );

    let number_hash_map =
        hash_mapify!(usize, "first_key" = 1, "second_variable" = 2, some_key = 3,);

    dbg!(hash_map);
    dbg!(number_hash_map);
}
