extern crate secp256k1;
use secp256k1::{PublicKey, Secp256k1, SecretKey};
use base64;
use hex;
use sha3::{Digest, Keccak256};

pub fn enter() {
    //get_eth_pubkey_from_secret();
    genrate_eth_address_from_secret();
}


fn genrate_eth_address_from_secret( ) {

    // address: 0x3fc370b8b16219e97f9428c15ec1e6e013b6b083
    let private_key_hex = "c9aec35ab193c7ae910addf90cd40eb3aca57b147ce7b13e2d9b44d848dacf25"; 
    let private_key_bytes = hex::decode(private_key_hex).expect("Invalid hex string");

    // str -> SecretKey
    let secp = Secp256k1::new();
    let secret_key = SecretKey::from_slice(&private_key_bytes).expect("Invalid private key");

    // SecretKey -> PublicKey
    let public_key = PublicKey::from_secret_key(&secp, &secret_key);

    // uncompressed serialize public key
    let pub_key_bytes = public_key.serialize_uncompressed();

    let mut hasher = Keccak256::new();
    hasher.update(&pub_key_bytes[1..]); //  remove 0x04 byte
    let result = hasher.finalize();

    // Get the last bytes
    let eth_address = &result[12..];
    assert_eq!(hex::encode(eth_address), "3fc370b8b16219e97f9428c15ec1e6e013b6b083".to_string());

}

fn genrate_eth_secret_and_address() {

     // Generate a pair of random keys 
     let secp = secp256k1::Secp256k1::new();

     let (secret_key, public_key) = secp.generate_keypair(&mut rand::thread_rng());
     println!("Secret_key: {}", secret_key.display_secret());
     println!("pubkey_key: {:?}", public_key);
 
 
     // serialize pubkey
     let pub_key_bytes = public_key.serialize_uncompressed();
     println!("Uncompressed serialize pubic key: {:?}", pub_key_bytes);

 
     // The Ethereum address is obtained by taking the last 20 bytes of the Keccak-256 hash of the public key.
     //
     let mut hasher = Keccak256::new();

     // Ethereum public keys do not include the leading 0x04
     hasher.update(&pub_key_bytes[1..]); 
     let result = hasher.finalize();
 
     // Get the last 20 bytes
     let eth_address = &result[12..];
     println!("Ethereum Address: 0x{}", hex::encode(eth_address));
}