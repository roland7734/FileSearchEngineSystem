# File Search Engine Architecture

The **File Search Engine** is a system designed to index, search, and retrieve files from a local file system. Users interact with the system through a web-based interface, enabling real-time searches based on file name, content, and metadata. This document provides an overview of the system architecture using the C4 model, including context, containers, components, and interactions.

## 1. System Context

The system enables users to search the local file system through a web-based application. The overall flow includes the following actors and systems:

- **Application User (Person)**: The user interacts with the system via a web application to search and retrieve files.
- **File Search Engine (Software System)**: This system handles the search operation by indexing and querying the local file system.
- **Local File System (Software System)**: The source of data where files are indexed and searched.

![System Context Diagram](architecture_assets/images/C4%20Context.drawio.png)

### System Flow:

1. The user searches for files by name, content, or metadata.
2. The File Search Engine retrieves necessary data from the local file system.
3. Search results are displayed in real-time via the web application.

## 2. Containers

The system is organized into four primary containers:

![Containers Diagram](architecture_assets/images/C4%20Container.drawio.png)

### 2.1 Web Application Container (Front End)

- **Technology**: React with TypeScript
- **Function**: Provides a user interface to search and view files in the local file system.
- **Interaction**: Communicates with the API container through WebSockets for real-time search.

### 2.2 API Container (Backend)

- **Technology**: C++ with [uWebSockets](https://github.com/uNetworking/uWebSockets)
- **Function**: Acts as the business layer between the web application and the database.
- **Interaction**:
  - Receives search requests from the web application.
  - Queries the database for matching files.
  - Sends search results back via WebSockets.

### 2.3 Database Container

- **Technology**: PostgreSQL
- **Function**: Stores metadata of indexed files.
- **Interaction**:
  - Stores and retrieves file metadata.
  - Supports full-text search for efficient querying.

### 2.4 Index Builder Container

- **Technology**: C++
- **Function**: Scans the local file system, extracts metadata, and populates the database.
- **Interaction**:
  - Reads from the local file system.
  - Writes file metadata to the PostgreSQL database.

## 3. Component Overview

### 3.1 Backend Components

![Backend Component Diagram](architecture_assets/images/C4%20Backend%20Component.drawio.png)

The API container is composed one primary component:

1. **Search (C++)**:
   - Collects search input from the web application.
   - Calls the needed search function to query to the database.
   - Executes the search query in the PostgreSQL database.
   - Retrieves matching records based on file name, content, and metadata.
   - Sends the search results back to the web application via WebSockets.

### 3.2 Indexer Components

![Indexer Component Diagram](architecture_assets/images/C4%20Indexer%20Component.drawio.png)

The Index Builder contains a key component:

1. **FileCrawler (C++)**:
   - Scans folders recursively to extract file information (e.g., name, content, metadata).
   - Updates the PostgreSQL database with the extracted data.

## 4. System Interaction Flow

1. **Startup Phase**:

   - The Index Builder scans the local file system and updates the PostgreSQL database with metadata.

2. **Search Process**:

   - The user inputs search criteria through the web application.
   - The API receives the search query.
   - The Search component queries the database and retrieves matching files.
   - Then it returns the results to the web application in real-time.

3. **Real-Time Updates**:

   - Future implementations may introduce a scheduled job to continuously track changes to the local file system and keep the database updated.

## 5. Technologies Used

| Container                  | Technology            |
| -------------------------- | --------------------- |
| Web Application (Frontend) | React with TypeScript |
| API (Backend)              | C++ with uWebSockets  |
| Database                   | PostgreSQL            |
| Index Builder              | C++                   |

## 6. Legend

![Legend](architecture_assets/images/C4%20Legend.drawio.png)

## 7. Conclusion

The **File Search Engine** provides an efficient way to search and retrieve files from a local system using a structured architecture with clearly defined containers and components. The system's design supports real-time search through WebSockets, enabling fast and responsive file queries.
