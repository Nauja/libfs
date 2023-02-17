/**
 * JS wrapper for the libfs library written in ANSI C.
 * 
 * This is written in TypeScript as to had some type checking
 * and prevent mistakes.
 */

/**
 * libfs is the module exported by emscripten for wrapping
 * the WASM binary.
 * 
 * This is the entrypoint or libfs.js, so we export all our
 * constants and classes here.
 */
declare let libfs: any;

/** @internal */
function push_array(buffer: Uint8Array, size: number): number {
    const ptr = libfs._malloc(size * Uint8Array.BYTES_PER_ELEMENT);
    libfs.HEAPU8.set(buffer, ptr);
    return ptr;
}

/** @internal */
function pop_array(ptr: number, buffer: Uint8Array, size: number) {
    for (let i = 0; i < size; ++i) {
        buffer[i] = libfs.HEAPU8[ptr + i];
    }
    libfs._free(ptr);
}

/** @internal */
function with_string(val: string, fun: any): any {
    const ptr = libfs.allocateUTF8(val);
    const result = fun(ptr);
    libfs._free(ptr);
    return result;
}

function fs_exist(path: string): boolean {
    return with_string(path, (ptr: any) => {
        return libfs._fs_exist(ptr) != 0;
    });
}

libfs.exist = fs_exist;
