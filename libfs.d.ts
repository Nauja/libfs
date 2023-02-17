import "emscripten";
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
export function fs_exist(path: string): boolean;

export interface LibfsModule extends EmscriptenModule {
    exist: typeof(fs_exist);
    FS: any;
    NODEFS: any;
}

declare const factory: () => Promise<LibfsModule>;
export default factory;