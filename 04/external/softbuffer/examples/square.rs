use std::mem::MaybeUninit;
use std::rc::Rc;
use winit::event::{ElementState, Event, KeyEvent, WindowEvent};
use winit::event_loop::{ControlFlow, EventLoop};
use winit::keyboard::{Key, NamedKey};
use winit::window::WindowBuilder;

fn main() {
    unsafe {
        umain();
    }
}

unsafe fn umain() {
    let event_loop = EventLoop::new().unwrap();
    let window = Rc::new(WindowBuilder::new().build(&event_loop).unwrap());

    let display = softbuffer_bindings::Display::new(&window).unwrap();
    let soft_window = softbuffer_bindings::Window::new(&window).unwrap();

    let context = softbuffer_bindings::Context::new(display);
    if context.is_null() {
        panic!("error");
    }
    let surface = softbuffer_bindings::Surface::new(&*context, soft_window);
    if surface.is_null() {
        panic!("error");
    }

    let mut flag = false;

    event_loop
        .run(move |event, elwt| {
            elwt.set_control_flow(ControlFlow::Wait);

            match event {
                Event::WindowEvent {
                    window_id,
                    event: WindowEvent::RedrawRequested,
                } if window_id == window.id() => {
                    let (width, height) = {
                        let size = window.inner_size();
                        (size.width, size.height)
                    };
                    if !(*surface).resize(width, height) {
                        panic!("error");
                    }

                    let mut buffer = MaybeUninit::<softbuffer_bindings::Buffer>::uninit();
                    if !(*surface).buffer_mut(buffer.as_mut_ptr()) {
                        panic!("error");
                    }
                    let mut buffer = buffer.assume_init();

                    let ptr = buffer.data();

                    redraw(ptr, width as usize, height as usize, flag);

                    if !buffer.present() {
                        panic!("error");
                    }
                }
                Event::WindowEvent {
                    event: WindowEvent::CloseRequested,
                    window_id,
                } if window_id == window.id() => {
                    elwt.exit();
                }
                Event::WindowEvent {
                    event:
                        WindowEvent::KeyboardInput {
                            event:
                                KeyEvent {
                                    state: ElementState::Pressed,
                                    logical_key: Key::Named(NamedKey::Space),
                                    ..
                                },
                            ..
                        },
                    window_id,
                } if window_id == window.id() => {
                    // Flip the rectangle flag and request a redraw to show the changed image
                    flag = !flag;
                    window.request_redraw();
                }
                _ => {}
            }
        })
        .unwrap();

    softbuffer_bindings::Surface::destroy(surface);
    softbuffer_bindings::Context::destroy(context);
}

unsafe fn redraw(buffer: *mut u32, width: usize, height: usize, flag: bool) {
    for y in 0..height {
        for x in 0..width {
            let value = if flag && x >= 100 && x < width - 100 && y >= 100 && y < height - 100 {
                0x00ffffff
            } else {
                let red = (x & 0xff) ^ (y & 0xff);
                let green = (x & 0x7f) ^ (y & 0x7f);
                let blue = (x & 0x3f) ^ (y & 0x3f);
                (blue | (green << 8) | (red << 16)) as u32
            };
            *buffer.add(y * width + x) = value;
        }
    }
}
