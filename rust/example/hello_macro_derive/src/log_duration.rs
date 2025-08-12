use proc_macro::TokenStream;
use quote::quote;
use syn::{parse_macro_input, ItemFn};

pub(crate) fn log_duration_impl(_args: TokenStream, input: TokenStream) -> TokenStream {
    // ItemFn: It means it is a function
    let input = parse_macro_input!(input as ItemFn);

    let ItemFn {
        // Function signature
        sig,
        vis,
        // Function body
        block,
        attrs,
    } = input;

    // Extract statements from function body
    let statements = block.stmts;

    let function_identifier = sig.ident.clone();

    quote!(
        // Reapply all other properties on this function.
        // The compiler will not include in this list the macro we are currently processing.
        #(#attrs)*
        // Restructure the function declaration
        #vis #sig {
        
            let __start = std::time::Instant::now();

            // Creates a new block whose body is the body of the function.
            let __result = {
                #(#statements)*
            };

            println!("{} 耗时 {}μs", stringify!(#function_identifier), __start.elapsed().as_micros());

            return __result;
        }
    )
    .into()
}