

use base64;
use base64::{Engine as _, engine::{self, general_purpose}, alphabet};

const CUSTOM_ENGINE: engine::GeneralPurpose =
    engine::GeneralPurpose::new(&alphabet::URL_SAFE, general_purpose::NO_PAD);
pub fn enter() {

    let b64 = general_purpose::STANDARD.encode(b"hello world~");
    println!("{}", b64);        // aGVsbG8gd29ybGR+

    let mut buf = String::new();
    general_purpose::STANDARD.encode_string(b"hello world~", &mut buf);
    println!("{}", buf);        // aGVsbG8gd29ybGR+

    buf.clear();
    CUSTOM_ENGINE.encode_string(b"hello internet~", &mut buf);
    println!("{}", buf);    // aGVsbG8gaW50ZXJuZXR-


    let bytes = general_purpose::STANDARD.decode("aGVsbG8gd29ybGR+").unwrap();
    println!("{:?}", bytes);    // [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 126]

    let mut buffer = [0; 64];
    let len = general_purpose::STANDARD.decode_slice("aGVsbG8gd29ybGR+Cg==", &mut buffer).unwrap();

    // len: 13, buffer: [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 126, 10]
    println!("len: {len}, buffer: {:?}", &buffer[..len]);  

}