SUBDIRS = apricots

bin_PROGRAM = apricots

@GENERATE_CHANGELOG_RULES@
CHANGELOG_GIT_FLAGS = --oneline
dist-hook: dist-ChangeLog

CPPCHECK_CMD = cppcheck --std=c++17 --error-exitcode=1

CLANG_FORMAT_CMD = clang-format -style=file -Werror -i -dry-run

.PHONY: cppcheck
cppcheck:
	${CPPCHECK_CMD} ${SUBDIRS}

.PHONY: clang-format
clang-format:
	find ${SUBDIRS} \( -name \*.h -o -name \*.cpp \) -print0 | xargs -0 -n1 ${CLANG_FORMAT_CMD}

check-local: cppcheck clang-format
