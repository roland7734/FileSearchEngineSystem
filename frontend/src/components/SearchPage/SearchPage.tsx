import React, { useEffect, useState } from "react";
import { TextField, Button, Box, Typography, Modal } from "@mui/material";
import { FormControl, InputLabel, MenuItem, Select } from "@mui/material";
import { searchFiles } from "../../api/search";
import { SearchAggregates, SearchResult } from "../../models/searchResult";
import SearchCard from "../SearchCard/SearchCard";
import { getSearchSuggestions } from "../../api/suggestions";
import { SearchSuggestions } from "../../models/searchSuggestions";
import SmartSearchInput from "../SmartSearchInput/SmartSearchInput";
import Aggregates from "../Aggregates/Aggregates";
import AccessTimeIcon from "@mui/icons-material/AccessTime";
import FolderIcon from "@mui/icons-material/Folder";
import CalendarTodayIcon from "@mui/icons-material/CalendarToday";

const SearchPage: React.FC = () => {
  const [query, setQuery] = useState("");
  const [suggestions, setSuggestions] = useState<SearchSuggestions | null>(
    null
  );
  const [results, setResults] = useState<SearchResult[]>([]);
  const [aggregates, setAggregates] = useState<SearchAggregates | null>(null);
  const [loading, setLoading] = useState(false);
  const [modalOpen, setModalOpen] = useState(false);
  const [modalMessage, setModalMessage] = useState("");
  const [strategy, setStrategy] = useState("none");
  const [widgets, setWidgets] = useState<string[]>([]);
  const [folders, setFolders] = useState<string[]>([]);

  const toggleButtons = (disable: boolean) => {
    const buttons = document.querySelectorAll("button");
    buttons.forEach((btn) => {
      (btn as HTMLButtonElement).disabled = disable;
    });
  };

  const fetchSuggestions = async () => {
    try {
      const data = await getSearchSuggestions();
      setSuggestions(data);
      console.log("Fetched suggestions:", data);
    } catch (error: any) {
      console.log("error" + error.data);
      setSuggestions(null);
    }
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
      const data = await searchFiles(query, strategy.toLowerCase());
      if (data.corrected_query && data.corrected_query !== query) {
        setQuery(data.corrected_query);
      }
      const dataSuggestions = await getSearchSuggestions();
      setResults(data.results);
      setAggregates(data.aggregates);
      setWidgets(data.widgets || []);
      setFolders(data.folders || []);
      setSuggestions(dataSuggestions);
      setModalMessage(
        "Search completed. Found " + data.results.length + " results."
      );
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

  useEffect(() => {
    fetchSuggestions();
  }, []);

  const getWidgetIcon = (widget: string) => {
    if (widget.toLowerCase().includes("recent"))
      return <AccessTimeIcon sx={{ color: "#1976d2", mr: 2 }} />;
    if (widget.toLowerCase().includes("year"))
      return <CalendarTodayIcon sx={{ color: "#1976d2", mr: 2 }} />;
    return <FolderIcon sx={{ color: "#1976d2", mr: 2 }} />;
  };

  return (
    <Box sx={{ padding: 3, textAlign: "left" }}>
      <FormControl fullWidth sx={{ margin: "auto", marginBottom: 2 }}>
        <InputLabel id="strategy-label">Correction Strategy</InputLabel>
        <Select
          labelId="strategy-label"
          id="strategy-select"
          value={strategy}
          label="Correction Strategy"
          onChange={(e) => setStrategy(e.target.value)}
        >
          <MenuItem value="norvig">Norvig</MenuItem>
          <MenuItem value="none">None</MenuItem>
        </Select>
      </FormControl>
      <SmartSearchInput
        suggestions={
          suggestions || {
            accesstime: [],
            content: [],
            mimetype: [],
            path: [],
            size: [],
          }
        }
        query={query}
        setQuery={setQuery}
      />
      <Box display="flex" justifyContent="center" gap={2} sx={{ marginTop: 3 }}>
        <Button variant="contained" onClick={handleSearch} disabled={loading}>
          {loading ? "Searching..." : "Search"}
        </Button>
        <Button variant="outlined" onClick={handleCancel} disabled={loading}>
          Cancel
        </Button>
      </Box>
      {widgets.length > 0 && (
        <Box mt={5}>
          <Typography variant="h5" fontWeight="bold" gutterBottom>
            Active Widgets
          </Typography>
          <Box display="flex" flexDirection="column" gap={2}>
            {widgets.map((widget, index) => (
              <Box
                key={index}
                display="flex"
                alignItems="center"
                sx={{
                  backgroundColor: "#fefefe",
                  borderLeft: "5px solid #1976d2",
                  padding: "16px",
                  borderRadius: "8px",
                  boxShadow: "0 3px 10px rgba(0,0,0,0.1)",
                  transition: "transform 0.2s ease-in-out",
                  "&:hover": {
                    transform: "translateX(4px)",
                    backgroundColor: "#f0f8ff",
                  },
                }}
              >
                {getWidgetIcon(widget)}
                <Typography variant="subtitle1" fontWeight="medium">
                  {widget}
                </Typography>
              </Box>
            ))}
          </Box>
        </Box>
      )}
      {folders.length > 0 && (
        <Box mt={5}>
          <Typography variant="h5" fontWeight="bold" gutterBottom>
            Most Frequent Folders
          </Typography>
          <Box display="flex" flexDirection="column" gap={2}>
            {folders.map((folder, index) => (
              <Box
                key={index}
                display="flex"
                alignItems="center"
                sx={{
                  backgroundColor: "#fafafa",
                  borderLeft: "5px solid #4caf50",
                  padding: "14px 16px",
                  borderRadius: "8px",
                  boxShadow: "0 3px 8px rgba(0,0,0,0.08)",
                  transition: "all 0.2s ease-in-out",
                  "&:hover": {
                    backgroundColor: "#f0fff4",
                    transform: "translateX(4px)",
                  },
                }}
              >
                <FolderIcon sx={{ color: "#4caf50", marginRight: 2 }} />
                <Typography variant="subtitle1" fontWeight="medium">
                  {folder}
                </Typography>
              </Box>
            ))}
          </Box>
        </Box>
      )}
      {results.length > 0 && aggregates && (
        <Box mt={4}>
          <Aggregates aggregates={aggregates} />
        </Box>
      )}
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
