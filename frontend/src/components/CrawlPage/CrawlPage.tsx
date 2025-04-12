import React, { useState } from "react";
import { TextField, Button, Box, Typography, Modal } from "@mui/material";
import { startCrawl } from "../../api/crawl";
import { CrawlRequest } from "../../models/crawlRequest";

const CrawlPage: React.FC = () => {
  const [basePath, setBasePath] = useState("");
  const [patterns, setPatterns] = useState<string[]>([""]);
  const [modalOpen, setModalOpen] = useState(false);
  const [modalMessage, setModalMessage] = useState("");
  const [loading, setLoading] = useState(false);
  const [crawlStarted, setCrawlStarted] = useState(false);

  const toggleButtons = (disable: boolean) => {
    const buttons = document.querySelectorAll("button");
    buttons.forEach((btn) => {
      (btn as HTMLButtonElement).disabled = disable;
    });
  };

  const handleAddPattern = () => setPatterns([...patterns, ""]);

  const handlePatternChange = (index: number, value: string) => {
    const newPatterns = [...patterns];
    newPatterns[index] = value;
    setPatterns(newPatterns);
  };

  const handleCrawl = async () => {
    const crawlRequest: CrawlRequest = {
      basePath,
      patterns: patterns.filter((p) => p.trim() !== ""),
    };

    setLoading(true);
    toggleButtons(true);
    setCrawlStarted(true);
    setModalMessage("Crawl started...");

    setModalOpen(true);

    try {
      const response = await startCrawl(crawlRequest);
      setModalMessage(response.message || "Crawl completed.");
    } catch (error: any) {
      console.error("Crawl failed:", error.message);
      setModalMessage("Crawl failed: " + error.message);
    } finally {
      setLoading(false);
      toggleButtons(false);
      setCrawlStarted(false);
    }
  };

  return (
    <Box sx={{ padding: 3, maxWidth: 600, margin: "auto" }}>
      <TextField
        label="Base Path"
        variant="outlined"
        fullWidth
        value={basePath}
        onChange={(e) => setBasePath(e.target.value)}
        sx={{ marginBottom: 2 }}
      />
      <Typography variant="body1">Patterns</Typography>
      {patterns.map((p, i) => (
        <Box key={i} sx={{ display: "flex", gap: 2, marginBottom: 2 }}>
          <TextField
            label={`Pattern ${i + 1}`}
            variant="outlined"
            fullWidth
            value={p}
            onChange={(e) => handlePatternChange(i, e.target.value)}
          />
        </Box>
      ))}
      <Button variant="outlined" onClick={handleAddPattern} disabled={loading}>
        Add Pattern
      </Button>
      <Box mt={3}>
        <Button variant="contained" onClick={handleCrawl} disabled={loading}>
          {loading ? "Crawling..." : "Start Crawling"}
        </Button>
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

export default CrawlPage;
