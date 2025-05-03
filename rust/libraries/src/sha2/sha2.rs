use sha2::{Sha256, Digest};
use std::fs::File;
use std::io;



pub fn enter() {
    sha_string();
    sha_digset();
    sha_file();
    sha_with_diffent_fn();
}

fn sha_string() {

    let mut hasher = Sha256::new();
    let data = b"Hello world!";
    hasher.update(data);
    // `input` can be called repeatedly and is generic over `AsRef<[u8]>`
    hasher.update("String data");

    // Note that calling `finalize()` consumes hasher
    let hash1 = hasher.finalize();

    // a25010bde57eeed8f42400871345867cbb6b98773653e0012c6273efd51e5696
    println!("Result: {:x}", hash1);


    let hash2 = Sha256::new()
    .chain_update(b"Hello world!")
    .chain_update("String data")
    .finalize();

    assert_eq!(hash1, hash2);
}

fn sha_digset() {
    let hash = Sha256::digest(b"my message");

    //ea38e30f75767d7e6c21eba85b14016646a3b60ade426ca966dac940a5db1bab
    println!("Result: {:x}", hash);
}

fn sha_file() {

    let  current_dir = std::env::current_dir().unwrap();

    // '~/myself/github/lang-learn/rust/libraries/flying'
    println!("current directory: {:?}", current_dir.display());

    let path = String::from("./config/note.md");
    let mut file = File::open(&path).unwrap();
    let mut hasher = Sha256::new();
    let n = io::copy(&mut file, &mut hasher).unwrap();
    let hash = hasher.finalize();

    println!("Path: {}", path); // ./config/note.md
    println!("Bytes processed: {}", n); // 19 
    println!("Hash value: {:x}", hash);  // 5881707e54b0112f901bc83a1ffbacac8fab74ea46a6f706a3efc5f7d4c1c625
}

fn sha_with_diffent_fn() {
    let mut buf1 = [0u8; 32];
    let mut buf2 = [0u8; 64];

    hash_password::<sha2::Sha256>("my_password", "abcd", &mut buf1);
    hash_password::<sha2::Sha512>("my_password", "abcd", &mut buf2);

}

fn hash_password<D: Digest>(password: &str, salt: &str, output: &mut [u8]) {
    let mut hasher = D::new();
    hasher.update(password.as_bytes());
    hasher.update(b"$");
    hasher.update(salt.as_bytes());
    output.copy_from_slice(hasher.finalize().as_slice())
}

