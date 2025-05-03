use crate::packages::{
    CrateName,
};

use crate::{
    envlogger::logger, base64::base64, rand::rand, serde::serde, sha2::sha2,tokio::tokio,bytes::bytes,anyhow::anyhow,
    itertools::itertools,bitflags::bitflags, byteorder::byteorder,futureutil::future,randchacha::randchacha,chrono::chrono,
    hex::hex,indexmap::indexmap,jsonrpccore::jsonrpccore,signalshook::signalshook,numcpus::numcpus,uriparse::uriparse,secp256k1::secp256k1,
};

pub  fn execute(name: &CrateName) {
  
    match name {
        
        CrateName::EnvLogger => {
            logger::enter();
        }
        CrateName::Base64 => {
            base64::enter();
        },
        CrateName::Rand => {
            rand::enter();
        },
        CrateName::Serde => {
            serde::enter();
        }
        CrateName::Sha2 => {
            sha2::enter();
        },
        CrateName::Tokio => {
            tokio::enter();
        },
        CrateName::Bytes => {
            bytes::enter();
        },
        CrateName::Anyhow => {
            anyhow::enter();
        },
        CrateName::IterTools => {
            itertools::enter();
        },
        CrateName::BitFlags => {
            bitflags::enter();
        },
        CrateName::ByteOrder => {
            byteorder::enter();
        },
        CrateName::FutureUtil => {
            future::enter();
        },
        CrateName::RandChacha => {
            randchacha::enter();
        },
        CrateName::Chrnon => {
            chrono::enter();
        },
        CrateName::Hex => {
            hex::enter();
        },
        CrateName::Indexmap => {
            indexmap::enter();
        },
        CrateName::JsonRpcCore => {
            jsonrpccore::enter();
        },
        CrateName::SignalHook => {
            signalshook::enter();
        },
        CrateName::NumCpus => {
            numcpus::enter();
        },
        CrateName::UriParse => {
            uriparse::enter();
        },
        CrateName::Secp256k1 => {
            secp256k1::enter();
        },


    }

}