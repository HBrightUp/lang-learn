

use std::{
    any::{
        Any, TypeId,
    },
    fmt::{
        Debug,
    }
};

use crate::include::person::Person;


// Determine whether the type of parameter s is String
fn any_is_string_method_1(s: &dyn Any) -> bool {
    TypeId::of::<String>() == s.type_id()
}

// Determine whether the type of parameter s is String
fn any_is_string_method_2(s: &dyn Any) -> bool {
    s.is::<String>()
}

// s is converted to String type
fn any_convert_to_string(s: &dyn Any) -> Option<&String>{
    s.downcast_ref::<String>() 
}

// Get the name of the type T
fn any_get_type_name<T>(_: &T) -> String {
    std::any::type_name::<T>().to_string()
}

// Identify specific types
fn any_to_real_type<T: Any + Debug>(value: &T) {
    let value_any = value as &dyn Any;

    if let Some(_string) = value_any.downcast_ref::<String>() {
        // String
    } else if let Some(Person { id:_, name:_, age:_ }) = value_any.downcast_ref::<Person>() {
        //Person
    } else {
        // other
    }
}

pub fn stdany() {

    let p = Person{
        id: 100,
        name: "Rust".to_string(),
        age: 20,
    };

    assert!(!any_is_string_method_1(&p));
    assert!(any_is_string_method_1(&p.name));
    
    assert!(!any_is_string_method_2(&p));
    assert!(any_is_string_method_2(&p.name));

    assert!(!any_convert_to_string(&p).is_some());
    assert_eq!(any_convert_to_string(&p.name), Some(&p.name));

    any_to_real_type(&p);
    any_to_real_type(&p.name);
    any_to_real_type(&30);

    assert_eq!("standard::include::person::Person", any_get_type_name(&p));
    assert_eq!("alloc::string::String", any_get_type_name(&p.name));
}
