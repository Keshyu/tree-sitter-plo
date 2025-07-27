import XCTest
import SwiftTreeSitter
import TreeSitterPlo

final class TreeSitterPloTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_plo())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Plo grammar")
    }
}
