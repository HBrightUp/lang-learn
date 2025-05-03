
use hex::FromHex;
use hex::ToHex;

pub fn enter() {

    // str -> hex str
    let hex_string = hex::encode("Hello, world!");
    println!("{hex_string}");   //  48656c6c6f2c20776f726c6421

    // hex str -> [u8]
    let decoded_string: Vec<u8> = hex::decode("48656c6c6f20776f726c6421").unwrap();
    println!("{:?}", decoded_string); // [72, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 33]

    // str -> hex  slice
    let mut bytes = [0u8; 4 * 2];
    hex::encode_to_slice(b"kiwi", &mut bytes).unwrap();
    assert_eq!(&bytes, b"6b697769");

    // hex slice -> str
    let mut bytes = [0u8; 4];
    assert_eq!(hex::decode_to_slice("6b697769", &mut bytes as &mut [u8]), Ok(()));
    assert_eq!(&bytes, b"kiwi");

    // hex str -> str
    let buffer = <[u8; 12]>::from_hex("48656c6c6f20776f726c6421").unwrap();
    let string = str::from_utf8(&buffer).expect("invalid buffer length");
    println!("{}", string); // Hello world!

    // str -> hex str
    println!("{}", "Hello world!".encode_hex::<String>());  //48656c6c6f20776f726c6421

}