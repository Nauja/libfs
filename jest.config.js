module.exports = {
    moduleFileExtensions: [
        "ts",
        "js"
    ],
    moduleDirectories: ["node_modules", "<rootDir>"],
    transform: {
        "^.+\\.(ts|tsx)$": ["ts-jest", {
            tsconfig: "tsconfig.json"
        }]
    },
    testMatch: [
        "**/tests/**/*.(ts|js)"
    ],
    testEnvironment: "node"
};