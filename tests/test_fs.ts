import factory, { LibfsModule } from "../libfs";

describe("test libfs", () => {
    /**
     * Initialize the filesystem for tests.
     * @param libfs - Libfs module
     */
    function initFS(libfs: LibfsModule): void {
        libfs.FS.mkdir('/working');
        libfs.FS.mount(libfs.NODEFS, { root: 'tests/data' }, '/working');
    }

    it("should see that hello.txt exists", (done) => {
        factory().then((libfs: LibfsModule) => {
            initFS(libfs);
            expect(libfs.exist("/working/hello.txt")).toBeTruthy();
            done();
        });
        //fs.mount(".");
    });
});