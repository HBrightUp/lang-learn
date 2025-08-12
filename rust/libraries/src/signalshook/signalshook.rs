use std::thread::JoinHandle;
use std::process::exit;
use std::fs::OpenOptions;
use log::{ error, info};

pub fn enter() {
    redirect_stderr_to_file(Some("./log.file".to_string()));
    error!("some error!");
    info!("I will exit.");
}

// When the program receives an interrupt signal, redirecting the standard output to a specified file 
// can help us better find the cause of the interruption.
pub fn redirect_stderr_to_file(logfile: Option<String>) -> Option<JoinHandle<()>> {

    if env::var_os("RUST_BACKTRACE").is_none() {
        env::set_var("RUST_BACKTRACE", "1");
    }

    // 
    let filter = "solana=info";
    match logfile {
        None => {
            None
        }
        Some(logfile) => {
            #[cfg(unix)]
            {
                use log::info;
                let mut signals =
                    signal_hook::iterator::Signals::new([signal_hook::consts::SIGUSR1])
                        .unwrap_or_else(|err| {
                            eprintln!("Unable to register SIGUSR1 handler: {err:?}");
                            exit(1);
                        });

                
                redirect_stderr(&logfile);
                Some(
                    std::thread::Builder::new()
                        .name("solSigUsr1".into())
                        .spawn(move || {
                            for signal in signals.forever() {
                                info!(
                                    "received SIGUSR1 ({}), reopening log file: {:?}",
                                    signal, logfile
                                );
                                redirect_stderr(&logfile);
                            }
                        })
                        .unwrap(),
                )
            }
            #[cfg(not(unix))]
            {
                println!("logrotate is not supported on this platform");
                solana_logger::setup_file_with_default(&logfile, filter);
                None
            }
        }
    }
}

fn redirect_stderr(filename: &str) {
    use std::os::unix::io::AsRawFd;
    match OpenOptions::new().create(true).append(true).open(filename) {
        Ok(file) => unsafe {
            libc::dup2(file.as_raw_fd(), libc::STDERR_FILENO);
        },
        Err(err) => eprintln!("Unable to open {filename}: {err}"),
    }
}