
use standard::std::{
    Modules, PrimitiveTypes,
};
use standard::cli::{
    execute_modules, execute_primitive_types,
};

fn main() {
    println!("Start learn std of rust.");

    execute_modules(Modules::StdArc);
    execute_primitive_types(PrimitiveTypes::Pointer);

}

