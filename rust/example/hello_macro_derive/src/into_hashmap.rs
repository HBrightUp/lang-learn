
use proc_macro::TokenStream;
use quote::quote;
use syn::Data;

// method-1:
// pub fn impl_into_hash_map( input: &syn::DeriveInput) -> TokenStream {
//     let struct_identifier = &input.ident;

//     match &input.data {
//         Data::Struct(syn::DataStruct { fields, .. }) => {
//             let mut implementation = quote!{
//                 let mut hash_map = std::collections::HashMap::<String, String>::new();
//             };

//             for field in fields {
//                 let identifier = field.ident.as_ref().unwrap();
//                 implementation.extend(quote!{
//                     hash_map.insert(stringify!(#identifier).to_string(), String::from(value.#identifier));
//                 });
//             }

//             quote! {
//                 #[automatically_derived]
//                 impl From<#struct_identifier> for std::collections::HashMap<String, String> {
//                     fn from(value: #struct_identifier) -> Self {
//                         #implementation

//                         hash_map
//                     }
//                 }
//             }
//         }
//         _ => unimplemented!()
//     }.into()
// }   


// method-2: Using iter to simplify expressions
pub fn impl_into_hash_map( input: &syn::DeriveInput) -> TokenStream {
    let struct_identifier = &input.ident;
    match &input.data {
        Data::Struct(syn::DataStruct { fields, .. }) => {
            let field_identifiers = fields.iter().map(|item| item.ident.as_ref().unwrap()).collect::<Vec<_>>();

            quote! {
                impl From<#struct_identifier> for std::collections::HashMap<String, String> {
                    fn from(value: #struct_identifier) -> Self {
                        let mut hash_map = std::collections::HashMap::<String, String>::new();

                        #(
                            hash_map.insert(stringify!(#field_identifiers).to_string(), String::from(value.#field_identifiers));
                        )*

                        hash_map
                    }
                }
            }
        }
        _ => unimplemented!()
    }.into()
}