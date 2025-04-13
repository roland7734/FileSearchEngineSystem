import React from "react";
import { Card, CardContent, Typography, Box } from "@mui/material";
import "./SearchCard.scss";
import { openFile } from "../../api/openFile";

interface SearchCardProps {
  path: string;
  preview: string;
}

const SearchCard: React.FC<SearchCardProps> = ({ path, preview }) => {
  const handleCardClick = async () => {
    try {
      await openFile(path);
    } catch (err) {
      alert("Failed to open file.");
    }
  };

  return (
    <Card
      className="search-card"
      onClick={handleCardClick}
      style={{ cursor: "pointer" }}
    >
      <CardContent>
        <Typography variant="h6" className="path">
          {path}
        </Typography>
        <Typography variant="body2" className="preview">
          {preview}
        </Typography>
      </CardContent>
    </Card>
  );
};

export default SearchCard;
