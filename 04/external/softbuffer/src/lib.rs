use std::num::{NonZeroIsize, NonZeroU32};
use std::cell::RefCell;

#[allow(unused_imports)]
use std::ffi::{c_int, c_ulong};

use raw_window_handle::{
    DisplayHandle, RawDisplayHandle,
    RawWindowHandle, Win32WindowHandle, WindowHandle, WindowsDisplayHandle,
};

macro_rules! cstr {
    ($str:literal) => {
        std::ffi::CStr::from_bytes_with_nul_unchecked(concat!($str, "\0").as_bytes())
    };
}

#[repr(C)]
pub struct Surface {
    display: RawDisplayHandle,
    window: RawWindowHandle,
    context: softbuffer::Context<DisplayHandle<'static>>,
    surface: softbuffer::Surface<DisplayHandle<'static>, WindowHandle<'static>>,
    buffer: Option<softbuffer::Buffer<'static, DisplayHandle<'static>, WindowHandle<'static>>>,
}

impl Surface {
    pub unsafe fn new(display: RawDisplayHandle, window: RawWindowHandle) -> *mut Self {
        let context = match softbuffer::Context::new(DisplayHandle::borrow_raw(display)) {
            Ok(context) => context,
            Err(error) => {
                handle_error(error);
                return std::ptr::null_mut();
            }
        };
        let surface = match softbuffer::Surface::new(&context, WindowHandle::borrow_raw(window)) {
            Ok(surface) => surface,
            Err(error) => {
                handle_error(error);
                return std::ptr::null_mut();
            }
        };
        Box::leak(Box::new(Surface {
            display,
            window,
            context,
            surface,
            buffer: None,
        })) as *mut _
    }

    pub unsafe fn get_buffer(&'static mut self) -> *mut u32 {
        match self.surface.buffer_mut() {
            Ok(mut buffer) => {
                let slice: &mut [u32] = &mut *buffer;
                let ptr = slice.as_mut_ptr();
                self.buffer.replace(buffer);
                ptr
            }
            Err(err) => {
                handle_error(err);
                std::ptr::null_mut()
            }
        }
    }

    pub unsafe fn len(&self) -> usize {
        match &self.buffer {
            Some(buf) => buf.len(),
            None => 0,
        }
    }

    pub fn resize(&mut self, width: u32, height: u32) -> bool {
        if width == 0 || height == 0 {
            return false;
        }
        self.buffer = None;
        let width = unsafe { NonZeroU32::new_unchecked(width) };
        let height = unsafe { NonZeroU32::new_unchecked(height) };
        let result = self.surface.resize(width, height);
        if let Err(err) = result {
            eprintln!("{err:?}");
            false
        } else {
            true
        }
    }

    pub fn present(&mut self) -> bool {
        match self.buffer.take() {
            Some(buffer) => match buffer.present() {
                Ok(()) => true,
                Err(error) => {
                    handle_error(error);
                    false
                }
            }
            None => {
                handle_no_buffer_error();
                false
            }
        }
    }

    pub unsafe fn destroy(this: *mut Self) {
        if this.is_null() {
            return;
        }
        drop(Box::from_raw(this));
    }
}

pub enum Error {
    SB(softbuffer::SoftBufferError),
    NoBuffer,
}

thread_local! {
    pub static LAST_ERROR: RefCell<Option<Error>> = RefCell::new(None);
}

fn handle_error(err: softbuffer::SoftBufferError) {
    LAST_ERROR.with(|global| {
        global.borrow_mut().replace(Error::SB(err));
    });
}

fn handle_no_buffer_error() {
    LAST_ERROR.with(|global| {
        global.borrow_mut().replace(Error::NoBuffer);
    });
}

fn get_last_error_msg() -> *const i8 {
    use raw_window_handle::HandleError::*;
    use softbuffer::SoftBufferError::*;
    use Error::*;

    LAST_ERROR.with(|global| {
        let opt_error = global.borrow();
        let Some(error) = &*opt_error else { return std::ptr::null(); };
        // SAFETY: None of the string literals contain null bytes
        unsafe {
            match error {
                SB(RawWindowHandle(NotSupported)) => cstr!("window handle not supported").as_ptr(),
                SB(RawWindowHandle(Unavailable)) => cstr!("window handle unavailable").as_ptr(),
                SB(UnsupportedDisplayPlatform { .. }) => cstr!("unsupported display platform").as_ptr(),
                SB(UnsupportedWindowPlatform { .. }) => cstr!("unsupported window platform").as_ptr(),
                SB(IncompleteWindowHandle) => cstr!("unsupported window platform").as_ptr(),
                SB(IncompleteDisplayHandle) => cstr!("incomplete window handle").as_ptr(),
                SB(SizeOutOfRange { .. }) => cstr!("size out of range").as_ptr(),
                SB(DamageOutOfRange { .. }) => cstr!("damage out of range").as_ptr(),
                SB(PlatformError(..)) => cstr!("platform error").as_ptr(),
                SB(Unimplemented) => cstr!("uninplemented").as_ptr(),
                SB(_) => cstr!("unexpected error").as_ptr(),
                NoBuffer => cstr!("no buffer").as_ptr(),
            }
        }
    })
}

#[cfg(target_os = "windows")]
#[no_mangle]
pub extern "C" fn sbSurfaceNew(hwnd: isize, hinstance: isize) -> *mut Surface {
    unsafe {
        let display = WindowsDisplayHandle::new();
        let mut whandle = Win32WindowHandle::new(NonZeroIsize::new_unchecked(hwnd));
        whandle.hinstance = NonZeroIsize::new(hinstance);
        Surface::new(display.into(), whandle.into())
    }
}

#[cfg(target_os = "linux")]
pub extern "C" fn sbSurfaceNew(x11_display: *mut c_void, screen: c_int, window: c_ulong) -> *mut Surface {
    unsafe {
        let display = XlibDisplayHandle::new(x11_display, screen);
        let whandle = XlibWindowHandle::new(window);
        Surface::new(display.into(), whandle.into())
    }
}

#[no_mangle]
pub extern "C" fn sbSurfaceDestroy(surface: *mut Surface) {
    unsafe { drop(Box::from_raw(surface)) }
}

#[no_mangle]
pub extern "C" fn sbSurfaceResize(surface: *mut Surface, width: u32, height: u32) -> bool {
    unsafe { (*surface).resize(width, height) }
}

#[no_mangle]
pub extern "C" fn sbSurfaceBuffer(surface: *mut Surface) -> *mut u32 {
    unsafe { (*surface).get_buffer() }
}

#[no_mangle]
pub extern "C" fn sbSurfaceSize(surface: *const Surface) -> usize {
    unsafe { (*surface).len() }
}

#[no_mangle]
pub extern "C" fn sbSurfacePresent(surface: *mut Surface) -> bool {
    unsafe { (*surface).present() }
}

#[no_mangle]
pub extern "C" fn sbErrorMessage() -> *const i8 {
    get_last_error_msg()
}
