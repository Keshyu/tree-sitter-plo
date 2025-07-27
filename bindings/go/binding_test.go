package tree_sitter_plo_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_plo "github.com/keshyu/tree-sitter-plo/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_plo.Language())
	if language == nil {
		t.Errorf("Error loading Plo grammar")
	}
}
