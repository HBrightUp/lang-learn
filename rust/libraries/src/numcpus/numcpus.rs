
pub fn enter() {
    let default_num_threads = num_cpus::get().to_string();
    println!("current num threads: {}", default_num_threads);
}