# Contributing to Aether

Thank you for your interest in helping build this interpreter! To maintain code quality and project stability, please follow these guidelines.

## 🤝 Code of Conduct
By participating, you agree to keep discussions professional and respectful.

## 🛠️ Development Environment
Before contributing, ensure you have the following installed to match our environment:
- **Build Tools:** `cmake`, `g++`, `make`
- **Formatter:** `clang-format` (Required for CI to pass)
  - *Debian/Ubuntu:* `sudo apt-get install clang-format`
  - *Fedora:* `sudo dnf install clang-tools-extra`

## 🧹 How to Contribute
1. **Branch:** Create your branch from `main`. Use a descriptive name (e.g., `git checkout -b feature/new-token-type` or `git checkout -b fix/lexer-crash`).
2. **Format Your Code:** Before committing, you **must** run the following command to ensure your code matches the project style. If you don't, the CI will automatically fail your Pull Request.
   ```bash
   find . -iname *.cpp -o -iname *.hpp | xargs clang-format -i
3. **Include Tests:** If you are adding a feature or fixing a bug, please include a sample .at file or update the test suite to verify your changes.
4. **Document Changes:** Update the README or inline comments if your change introduces new functionality or changes existing behavior.

## 🚀 Pull Request Process
- **Fill the Template:** Complete the PR template provided by GitHub so we understand the "Why" behind your changes.
- **Descriptive Titles:** Ensure your PR title is clear (e.g., "Fix: resolve memory leak in Lexer string buffer").
- **CI Check:** All GitHub Actions must be green.
- **Approval:** Your PR must be approved by @Rajdeep-Nemo (for code) and @EL-STRIX (for any documentation changes) before it can be merged.