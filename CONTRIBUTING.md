## Contributing

We welcome contributions of all kinds! Whether it’s fixing bugs, adding new allocator backends, improving tests, or enhancing documentation, you’re encouraged to participate.


### How to Contribute

**1: Fork the repository** and create a feature branch:
```bash
git checkout -b my-feature
```

**2: Make your changes**

- For code changes, follow the existing project structure.

- Write unit tests for any new features or bug fixes.

- Ensure your code compiles cleanly on both desktop and Arduino (if applicable).

**3: Run tests on desktop:**
Add any new source files to the Makefile if needed, then run:
```bash
make run
```

**4: Commit your changes with a clear message:**
```bash
git commit -am "Add new pool allocator backend"
```

**5: Push to your fork:**
```bash
git push origin my-feature
```

**6: Open a Pull Request**
Describe your changes clearly and reference any relevant issues.

### Guidelines

- Follow existing code style and conventions.
- Keep functions small, modular, and well-documented.
- Use meaningful commit messages.
- Ensure all unit tests pass before submitting.