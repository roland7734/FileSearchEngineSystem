import React from "react";
import { AppBar, Toolbar, Typography, Button, Box } from "@mui/material";
import { useNavigate } from "react-router-dom";
import "./Header.scss";

const Header: React.FC = () => {
  const navigate = useNavigate();

  return (
    <AppBar position="static" className="header">
      <Toolbar className="toolbar">
        <Typography variant="h6" className="title">
          File Search Engine
        </Typography>
        <Box className="nav-buttons">
          <Button color="inherit" onClick={() => navigate("/")}>
            Home
          </Button>
          <Button color="inherit" onClick={() => navigate("/search")}>
            Search
          </Button>
          <Button color="inherit" onClick={() => navigate("/crawl")}>
            Crawl
          </Button>
          <Button color="inherit" onClick={() => navigate("/strategy")}>
            Strategy
          </Button>
        </Box>
      </Toolbar>
    </AppBar>
  );
};

export default Header;
