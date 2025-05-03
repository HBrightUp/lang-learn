use serde_derive::*;


//Info:  source from bitflags::example
use crate::bitflags::{
    macro_free, fmt, custom_bits_type,
};



pub fn enter() {
    bitflags_serde();
    macro_free::macro_free();
    let _ = fmt::fmt();
    custom_bits_type::custom_bits_type();
}


pub fn bitflags_serde() {
    bitflags::bitflags! {
        #[derive(Serialize, Deserialize, Debug, PartialEq, Eq)]
        #[serde(transparent)]
        pub struct Flags: u32 {
            const A = 1;
            const B = 2;
            const C = 4;
            const D = 8;
        }
    }

    let flags = Flags::A | Flags::B;
    let serialized = serde_json::to_string(&flags).unwrap();
    println!("{:?} -> {}", flags, serialized);
    assert_eq!(serialized, r#""A | B""#);

    let deserialized: Flags = serde_json::from_str(&serialized).unwrap();
    println!("{} -> {:?}", serialized, flags);
    assert_eq!(deserialized, flags);

}