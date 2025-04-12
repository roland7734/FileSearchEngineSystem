import React from "react";
import { AppBar, Toolbar, Typography, Button, Container } from "@mui/material";
import { useNavigate } from "react-router-dom";
import "./Header.scss";

const Header: React.FC = () => {
  const navigate = useNavigate();

  return (
    <AppBar position="static" className="header">
      <Container maxWidth="lg">
        <Toolbar className="toolbar">
          <Typography variant="h6" className="title">
            File Search Engine
          </Typography>
          <Button color="inherit" onClick={() => navigate("/")}>
            Home
          </Button>
          <Button color="inherit" onClick={() => navigate("/search")}>
            Search
          </Button>
          <Button color="inherit" onClick={() => navigate("/crawl")}>
            Crawl
          </Button>
        </Toolbar>
      </Container>
    </AppBar>
  );
};

export default Header;
