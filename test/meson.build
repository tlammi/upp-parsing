
deps = [upp_parsing_dep, dependency('gtest'), dependency('gtest_main')]

test_state = executable('test_state', ['test_state.cpp'], dependencies: deps)
test_term = executable('test_term', ['test_term.cpp'], dependencies: deps)
test_nonterm = executable('test_nonterm', ['test_nonterm.cpp'], dependencies: deps)

test('State', test_state)
test('Terminal', test_term)
test('Nonterminal', test_nonterm)
