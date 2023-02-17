const gap = require('gulp-append-prepend');
const gulp = require("gulp");
const replace = require('gulp-replace');
const rename = require("gulp-rename");
const ts = require('gulp-typescript');

const dev_build = ((process.env.NODE_ENV || 'development').trim().toLowerCase() === 'development');

/** Compile fs.ts. */
gulp.task("compile-ts", function () {
    return gulp
        .src("./fs.ts")
        .pipe(ts.createProject('tsconfig.json')())
        .pipe(gulp.dest("./dist"));
});

gulp.task("copy-types", function () {
    return gulp
        .src("./dist/fs.d.ts")
        .pipe(replace("declare let libfs: any;\n", ""))
        .pipe(replace("/** Pointer to the C instance. */\n    ", ""))
        .pipe(replace("protected _ptr?: number;\n    ", ""))
        .pipe(replace("private _ptr?;\n    ", ""))
        .pipe(replace("private _memory?;\n    ", ""))
        .pipe(replace("protected _manage_memory: boolean;\n    ", ""))
        .pipe(replace("get ptr(): number | undefined;\n    ", ""))
        .pipe(replace("constructor(ptr?: number);", "constructor();"))
        .pipe(replace("declare", "export"))
        .pipe(gap.prependText('import "emscripten";\n\n'))
        .pipe(gap.appendText(
            '\n\nexport interface LibfsModule extends EmscriptenModule {\n' +
            '    exist: typeof(fs_exist);\n' +
            '    FS: any;\n' +
            '    NODEFS: any;\n' +
            '}\n' +
            '\n' +
            'declare const factory: () => Promise<LibfsModule>;\n' +
            'export default factory;\n'))
        .pipe(rename("libfs.d.ts"))
        .pipe(gulp.dest("./"));
});

// Build everything
gulp.task("build", gulp.series("compile-ts", "copy-types"));