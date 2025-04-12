import React from "react";
import { Card, CardContent, Typography, Box } from "@mui/material";
import "./SearchCard.scss";

interface SearchCardProps {
  path: string;
  preview: string;
}

const SearchCard: React.FC<SearchCardProps> = ({ path, preview }) => {
  return (
    <Card className="search-card" onClick={() => window.open(path)}>
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
