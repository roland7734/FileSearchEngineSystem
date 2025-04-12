import React, { useState } from "react";
import { TextField, Button, Box, Typography, Modal } from "@mui/material";
import { searchFiles } from "../../api/search";
import { SearchResult } from "../../models/searchResult";
import SearchCard from "../SearchCard/SearchCard";

const SearchPage: React.FC = () => {
  const [query, setQuery] = useState("");
  const [results, setResults] = useState<SearchResult[]>([]);
  const [loading, setLoading] = useState(false);
  const [modalOpen, setModalOpen] = useState(false);
  const [modalMessage, setModalMessage] = useState("");

  const toggleButtons = (disable: boolean) => {
    const buttons = document.querySelectorAll("button");
    buttons.forEach((btn) => {
      (btn as HTMLButtonElement).disabled = disable;
    });
  };

  const handleSearch = async () => {
    if (query.trim() === "") {
      setModalMessage("Query cannot be empty.");
      setModalOpen(true);
      return;
    }

    setLoading(true);
    toggleButtons(true);
    try {
      const data = await searchFiles(query);
      setResults(data.results);
      setModalMessage("Search completed.");
    } catch (error: any) {
      console.error("Search failed:", error.message);
      setModalMessage("Search failed: " + error.message);
    } finally {
      setLoading(false);
      toggleButtons(false);
      setModalOpen(true);
    }
  };

  const handleCancel = () => {
    setQuery("");
    setResults([]);
  };

  return (
    <Box sx={{ padding: 3, textAlign: "center" }}>
      <TextField
        label="Search Query"
        variant="outlined"
        fullWidth
        value={query}
        onChange={(e) => setQuery(e.target.value)}
        sx={{ marginBottom: 2 }}
      />
      <Box display="flex" justifyContent="center" gap={2}>
        <Button variant="contained" onClick={handleSearch} disabled={loading}>
          {loading ? "Searching..." : "Search"}
        </Button>
        <Button variant="outlined" onClick={handleCancel} disabled={loading}>
          Cancel
        </Button>
      </Box>
      <Box mt={4}>
        {results.map((r, i) => (
          <SearchCard key={i} path={r.path} preview={r.preview} />
        ))}
      </Box>
      <Modal open={modalOpen} onClose={() => setModalOpen(false)}>
        <Box
          sx={{
            backgroundColor: "white",
            padding: 4,
            margin: "auto",
            marginTop: "20vh",
            width: 300,
            borderRadius: 2,
          }}
        >
          <Typography variant="h6">{modalMessage}</Typography>
        </Box>
      </Modal>
    </Box>
  );
};

export default SearchPage;
