# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Table of Contents

- [v2.0.0 - 2025-05-23](#200---2025-05-24)
- [v1.0.0 - 2025-04-14](#100---2025-04-14)
- [v0.1.0 - 2025-03-14](#010---2025-03-14)


---

## [2.0.0] - 2025-05-24
[2.0.0]: https://github.com/roland7734/FileSearchEngineSystem/releases/tag/v2.0.0

### Added

#### Spelling Correction System
- Integrated a robust **Spelling Correction Framework** leveraging the **Strategy Design Pattern**.
- Two strategies implemented:
  - `NoCorrection` – performs no alterations on queries.
  - `NorvigSpellingCorrector` – corrects misspelled content words using Peter Norvig’s algorithm.
- Refactored the backend parser and controller logic to support spelling correction per user selection.
- Introduced UI changes to display and allow user interaction with spelling suggestions via a **ComboBox** in the search interface.

#### Cache Systems for Performance Optimization
- Introduced two caching layers:
  - **LRU Cache**: Caches recent query results to reduce redundant DB calls.
  - **Serialized Cache Generator**: Serializes word frequencies used by the spelling corrector, significantly reducing startup time.

#### Widget System and Metadata Summary
- Created a **WidgetFactory** using the **Factory Pattern** to dynamically activate relevant widgets based on query context and metadata.
- Widgets added:
  - **Folder Widget** – visualizes directory structure.
  - **Histogram Widget** – displays monthly distribution of modified files using React Chart.js.
  - **Metadata Summary Widget** – shows file type and year distribution of current result set.
- Backend logic augmented to include widget data in JSON responses.

#### API Enhancements
- Modified `/search` route to:
  - Return widget suggestions.
  - Include summary metadata for improved front-end display.
- Introduced new endpoints for aggregate analysis of result sets (file types, date distribution, etc.).

#### Frontend Visual Enhancements
- Integrated **Material UI Icons** (e.g., `AccessTime`, `Folder`, `CalendarToday`) to improve result and widget display.
- Implemented responsive card-based layouts for widgets.
- Added hover and spacing effects to enhance visual structure.

#### Developer Experience & DevOps
- Added a **pre-commit Git hook** that:
  - Automatically builds the test suite.
  - Runs all unit tests and fails the commit if any fail.
  - Searches for TODOs in code, excluding external dependencies.
- Tag `v2.0.0` created to mark the milestone of this iteration.
- Updated `CHANGELOG.md` to document this release following [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) conventions.


### Changed

- Updated search logic to conditionally apply spelling correction only for valid `content:` filters.
- `/search` now handles and forwards widget suggestions to the front-end.
- Modified metadata analysis to handle inconsistent or malformed date strings more gracefully.
- Adjusted spelling correction logic to exclude `path`, `size`, `mimetype`, and `accesstime` fields.
- Optimized spelling correction by using pre-built serialized frequency dictionaries.


### Fixed

- Resolved parsing issues in `AccessTimeFilter` for malformed or ambiguous date formats.
- Corrected React rendering issues in metadata and widget displays.
- Fixed spelling correction logic to avoid over-correction of numeric values or special character sequences.


### Removed

- Deprecated hardcoded frequency calculation at startup; now replaced by cache loading.
- Removed redundant metadata parsing in old search result formatter.

### DevOps
- Implemented a Git pre-commit hook that:
  - Rebuilds the test suite.
  - Runs all unit tests.
  - Prevents commits if any test fails.
  - Checks for TODO comments in source files (excluding external headers).

### Summary

This iteration introduced **intelligent spelling correction**, **query-aware widget rendering**, and **comprehensive caching** mechanisms, marking a significant leap in the system's UX, performance, and modularity. The backend has grown more extensible, the frontend is smarter and richer, and the engine’s search intelligence is now equipped to suggest, correct, and visualize with precision.

Git tag `v2.0.0` created at the end of this iteration to represent a stable and feature-rich version.


## [1.0.0] - 2025-04-14
[1.0.0]: https://github.com/roland7734/FileSearchEngineSystem/releases/tag/v1.0.0

### Added

#### Search Ranking System
- Implemented a robust file scoring mechanism to rank results by relevance.
  - Criteria includes: file path depth, keyword presence in path, directory importance, file extension weight, file size, and recency of access.
  - Score is computed in `File::computeScore`, influencing SQL ordering logic.
- Extended backend search services to include this score during retrieval and result sorting.

#### Ranking Test Suite
- Added unit tests covering multiple ranking aspects:
  - Relative score ordering.
  - Preservation of insertion order when scores are equal.
  - Verification that top 5 highest-ranked files are returned.
  - Tests integrated with `InsertService` and `SearchService`.
  - Database is truncated before each test to isolate data environments.

#### Filter and Query Parser System
- Introduced modular filters using an `IFilter` interface and subclasses:
  - `ContentFilter`, `PathNameFilter`, `AccessTimeFilter`, `SizeFilter`, `MimeTypeFilter`.
- Created a powerful `QueryParser`:
  - Handles mixed key-value filters (`path:`, `content:`, `size:`, `accesstime:`, `mimetype:`).
  - Supports quoted values and comparison operators like `>`, `<`, `=`.
  - Error handling added for malformed or missing filter values.

#### Query Suggestions via Observer Pattern
- Added `SearchHistory` and `ResultsHistory` as observers to track:
  - Frequency of queries and their results.
  - Result usage (search and access counts).
- Integrated observer pattern into `SearchService` for real-time tracking and analytics.

#### Smart Autocomplete Bar
- Added a dynamic smart input component in the frontend:
  - Fetches and suggests past queries using `/suggestions` endpoint.
  - Reactively updates based on `SearchHistory`.

#### File Access Integration
- Implemented endpoint to open a file on the user's machine from the search results.
  - Logs access and updates file stats in the database.

#### Logging System (TXT + HTML)
- Added dynamic logging system with runtime strategy selection:
  - Plain text and HTML formats supported.
- Created LoggerManager and LoggerController.
- Logging strategy and filename can be changed via REST API.
- Logs are now separated by event type (crawl, search, strategy change).

#### Web Interface Expansion
- Added frontend pages for:
  - Home: project overview.
  - Search: query input, result previews.
  - Crawl: start a crawl session with patterns.
  - Strategy: switch between logging modes.
- Each page is modularized and stylized using Material UI components.

#### API Controllers
- Implemented `CrawlServiceController` and `SearchServiceController`:
  - Handle all backend communication via HTTP.
- Added `FileOpenController`, `LoggerController`, `QuerySuggestionsController` for modular logic separation and extensibility.

#### Configuration & Deployment
- Introduced `config.json` and utilities to dynamically configure the application.
- Enhanced CMake build with vcpkg and test support.

### Changed

- Refactored the main backend flow to include controller registration and modular service instantiation.
- Split concerns across multiple folders: `controller/`, `filters/`, `service/`, `model/`.
- Improved frontend style and user experience using SCSS and new Material UI elements.
- Enhanced CORS support for all routes.

### Fixed

- Improved error handling in:
  - `QueryParser`: catches invalid keys, missing values, or broken quoted strings.
  - `InsertService`: validates file paths before inserting to DB.
- Corrected UI bugs:
  - Placeholder UI elements now render conditionally.
  - Fixed routing issues on direct page reloads.

### Removed

- Deprecated static logging code in favor of new dynamic system.
- Removed legacy `main` functions and commented boilerplate.
- Removed test data and unused scripts.

### Summary

This iteration significantly elevated the capabilities of the File Search Engine System, transforming it from a basic search tool into a robust, user-responsive platform with:

- Intelligent ranking.
- Query history-driven suggestions.
- File metadata awareness.
- Smart frontend UX.
- Expandable REST architecture.
- Advanced test coverage and reliability.

The milestone concludes with a stable and polished version — `v1.0.0`.
This release marks a significant leap in the intelligence and interactivity of the File Search Engine. By integrating a sophisticated ranking engine, powerful query syntax, and modular logging infrastructure, **Iteration 2** lays the groundwork for delivering personalized and efficient file discovery across massive datasets.


## 0.1.0 - 2025-03-14

### Added

#### Core Backend Architecture
- Created foundational components of the file search engine including:
  - `file-crawler` for directory traversal and metadata extraction.
  - `insertService` to support efficient batch insertion of files into the database.
  - `file.hpp` and `file.cpp` to represent internal file models with relevant metadata.
  - `database.hpp/.cpp` to manage PostgreSQL connection setup and lifecycle.
- Developed `FileIgnorer` for ignoring files using customizable patterns.
  - Initially implemented using regex, later simplified for efficiency via string matching.
- Implemented the first version of `SearchService` for querying files by content and name.
- Implemented the first version of `Indexer` (previously `IndexBuilder`) for managing file indexing logic.
- Enabled support for `.txt` file preview generation during the crawling process.

#### Configuration and Logging
- Introduced a global logger for tracking:
  - Crawl start/end.
  - Files indexed or ignored.
  - Indexing duration.
  - Search times and user input.
- Developed JSON-based configuration system:
  - Added `config.json` for centralized settings such as DB credentials and crawler rules.
  - `config.hpp` and `Config.cpp` were created to load and validate config values dynamically.

#### UTF-8 Handling and String Processing
- Added `StringProcessor` class:
  - `cleanInvalidUtf8Characters`: Cleans invalid UTF-8 bytes.
  - `WideToUtf8`: Converts wide strings to UTF-8 safely for use in logs and database.

#### Documentation and Diagrams
- Created `ARCHITECTURE.md` describing the structure and logic of the system.
- Added supporting assets:
  - C4 diagrams for container and component views.
  - Clear visual links showing how modules interact.

### Changed

- Refactored:
  - Renamed `IndexBuilder` to `Indexer` for naming clarity.
  - Replaced complex regex filtering logic with faster string-based filtering in `FileIgnorer`.
- Improved error handling in `InsertService` and `SearchService`:
  - Clear, robust SQL exception capture and messaging.
  - Safer database interaction, reducing the risk of crashes or silent failures.

### Fixed

- Improved SQL error messages across services to better reflect failure causes and improve debugging.
- Corrected UTF-8 decoding issues in large file previews.
- Ensured file previews are truncated and sanitized before insertion to prevent DB overflow.
- Resolved early crashes due to malformed configuration or missing values.

### Removed

- Eliminated deprecated `getFilesFromDirectory()` method in the crawler.
- Removed early prototype classes that duplicated new modules' responsibilities.
- Stripped temporary test scaffolding used before logging and config loading were completed.

### Build

- Integrated vcpkg for C++ dependency management.
- Setup build structure with `cmake-build-debug` directory and linked PostgreSQL via `libpqxx`.

### Summary

This lays the **foundation** for the File Search Engine System. It integrates:
- Robust **file crawling**
- **Metadata extraction**
- **Database storage**
- **Basic full-text search**
- Clear **logging**
- Initial **architecture documentation**

It establishes the platform for future expansion into query filtering, result scoring, caching, suggestion systems, and a full frontend experience.




