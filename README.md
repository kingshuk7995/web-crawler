# WebCrawler

- A simple web search engine project

### 🎯 Motivation
- To understand the basic working of a search engine

### 🚀 Features
- Fully asynchronous and scallable
- Fetches Html from the internet parses and stores them
- Search queries with GUI

### 🛠️ Tech Stack & Libraries
- **[Drogon: A modern high performance C++ Web framework](https://github.com/drogonframework/drogon)**
- **[Gumbo Parser: C based HTML parser](https://github.com/google/gumbo-parser)**
- **[PostgreSQL: Relational database](https://www.postgresql.org/)**
### 🧩 Project Overview
- Used C++ drogon framework for the HTTP server.
- Used C++ drogon::HttpClient to get Html content from internet.
- Used Gumbo C++ for parsing Html files and getting the meta data from it.
- Used postgreSQL as database for storing the parsed data.
- Used Drogon's CSP(C++ server pages) for UI.

  # deprecated Realising C++ isnt the good choice unless building ground up with boost::asio and Gumbo and DataBase
