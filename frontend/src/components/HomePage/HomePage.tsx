import React from "react";
import { Box, Typography } from "@mui/material";

const HomePage: React.FC = () => {
  return (
    <Box
      display="flex"
      flexDirection="column"
      alignItems="center"
      justifyContent="center"
      height="70vh"
    >
      <Typography variant="h3">Welcome to the File Search Engine</Typography>
      <Typography variant="h6" mt={2}>
        Use this platform to search and crawl files efficiently.
      </Typography>
    </Box>
  );
};

export default HomePage;
