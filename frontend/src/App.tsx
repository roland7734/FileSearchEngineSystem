import React from "react";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Header from "./components/Header/Header";
import CrawlPage from "./components/CrawlPage/CrawlPage";
import HomePage from "./components/HomePage/HomePage";
import SearchPage from "./components/SearchPage/SearchPage";
import StrategyPage from "./components/StrategyPage/StrategyPage";

const App: React.FC = () => {
  return (
    <Router>
      <Header />
      <Routes>
        <Route path="/" element={<HomePage />} />
        <Route path="/search" element={<SearchPage />} />
        <Route path="/crawl" element={<CrawlPage />} />
        <Route path="/strategy" element={<StrategyPage />} />
      </Routes>
    </Router>
  );
};

export default App;
