use proc_macro::TokenStream;
use quote::{quote, ToTokens};
use syn::parse::{Parse, ParseStream};
use syn::{parse_macro_input, Lit, LitStr, Token, Type};
/*
    To simplify the process, which can easily become complex, 
    we only support primitive types such as strings, integers, floating point numbers, and booleans. 
    Therefore, we do not support creating hash_maps with non-string keys or with enumeration and struct values.
 */

// proc_macro2: There may be multiple types of input
pub struct ParsedMapEntry(String, proc_macro2::TokenStream);

//Custom data structures
pub struct ParsedMap {
    value_type: Type,
    entries: Vec<ParsedMapEntry>,
}

impl ToTokens for ParsedMapEntry {
    fn to_tokens(&self, tokens: &mut proc_macro2::TokenStream) {
        let key = self.0.clone();
        let value = self.1.clone();

        tokens.extend(quote!(String::from(#key), #value));
    }
}

impl Parse for ParsedMap {
    fn parse(input: ParseStream) -> syn::Result<Self> {
        let mut entries = Vec::<ParsedMapEntry>::new();

        if input.is_empty() {
            panic!("Missing hashmap specifies a type.");
        }

        let ty = input.parse::<Type>()?;

        // Parse the `,` token, which you would expect to be used to separate arguments.
        input.parse::<Token![,]>()?;

        while !input.is_empty() {

            //Supports identifiers and string literals,
            let key = if let Ok(key) = input.parse::<syn::Ident>() {
                key.to_string()
            } else if let Ok(key) = input.parse::<LitStr>() {
                key.value()
            } else {
                panic!("Keys must be string literals or identifiers!");
            };

            // Parse the `=` symbol, which should be the next token after the key.
            input.parse::<Token![=]>()?;

            // Next, try to resolve the value as an identifier.
            // Variables are directly converted into token streams.
            let value = if let Ok(value) = input.parse::<syn::Ident>() {
                value.to_token_stream()
                // Literal value is directly converted into token streams.
            } else if let Ok(value) = input.parse::<Lit>() {
                value.to_token_stream()
            } else {
                // If the input is neither an identifier nor a literal value, an error is reported.
                panic!("Value must be a literal or an identifier!");
            };

            entries.push(ParsedMapEntry(key, value));

            // Check if the next token is a ',' 
            if input.peek(Token![,]) {
                // If it is, parse it out and advance the stream, then continue processing the next key-value pair
                input.parse::<Token![,]>()?;
            }
        }

        Ok(ParsedMap {
            value_type: ty,
            entries,
        })
    }
}

pub fn hash_mapify_impl(item: TokenStream) -> TokenStream {

    // Parse the input token stream into the `ParsedMap` we defined。
    let input = parse_macro_input!(item as ParsedMap);

    let key_value_pairs = input.entries;
    let ty = input.value_type;

    quote!({
        // Creates a new hash map with `String` as key type and `#ty` parsed from the macro input arguments as value type.
        let mut hash_map = std::collections::HashMap::<String, #ty>::new();
        #(
            hash_map.insert(#key_value_pairs);
        )*

        hash_map
    })
    .into()
}