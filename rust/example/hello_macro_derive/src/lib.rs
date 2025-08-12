#![feature(proc_macro_quote)]

use proc_macro::TokenStream;
use quote::quote;
use syn::Data;

mod hello;
mod into_hashmap;
mod custom_model;
mod log_duration;
mod constant_string;
mod hash_mapify;



//Info: Only functions of type proc_macro_derive may be defined in this file.
#[proc_macro_derive(HelloMacro)]
pub fn hello_macro_derive(input: TokenStream) -> TokenStream {

    let ast: syn::DeriveInput = syn::parse(input).unwrap();
    hello::impl_hello_macro(&ast)
}

#[proc_macro_derive(IntoHashMap)]
pub fn into_hash_map(item: TokenStream) -> TokenStream {

    let input = syn::parse_macro_input!(item as syn::DeriveInput);
    into_hashmap::impl_into_hash_map(&input)
}

#[proc_macro_derive(DeriveCustomModel, attributes(custom_model))]
pub fn derive_custom_model(item: TokenStream) -> TokenStream {
    custom_model::custom_model_impl(item)
}

#[proc_macro_attribute]
pub fn log_duration(args: TokenStream, item: TokenStream) -> TokenStream {
    log_duration::log_duration_impl(args, item) 
}

#[proc_macro]
pub fn constant_string(item: TokenStream) -> TokenStream {
    constant_string::constant_string_impl(item)
}

#[proc_macro]
pub fn hash_mapify(item: TokenStream) -> TokenStream {
    hash_mapify::hash_mapify_impl(item)
}