import React, { useState } from "react";
import {
  TextField,
  Button,
  Container,
  Box,
  Typography,
  MenuItem,
  Select,
  InputLabel,
  FormControl,
} from "@mui/material";
import { useNavigate } from "react-router-dom";
import { changeLoggerStrategy } from "../../api/changeStrategy";
import { LoggerStrategyRequest } from "../../models/loggerStrategyRequest";

const StrategyPage: React.FC = () => {
  const navigate = useNavigate();
  const [loggerType, setLoggerType] = useState<"html" | "text">("html");
  const [filePath, setFilePath] = useState<string>("");
  const [message, setMessage] = useState<string>("");

  const handleStrategyChange = async () => {
    const strategyRequest: LoggerStrategyRequest = {
      type: loggerType,
      path: filePath,
    };

    try {
      const response = await changeLoggerStrategy(strategyRequest);
      setMessage(`Logger strategy changed to ${response.type}`);
    } catch (error: any) {
      setMessage(`Error: ${error.message}`);
    }
  };

  return (
    <Container maxWidth="sm">
      <Box my={4}>
        <Typography variant="h4" gutterBottom>
          Change Logger Strategy
        </Typography>

        <FormControl fullWidth variant="outlined" margin="normal">
          <InputLabel id="logger-type-label">Logger Type</InputLabel>
          <Select
            labelId="logger-type-label"
            label="Logger Type"
            value={loggerType}
            onChange={(e) => setLoggerType(e.target.value as "html" | "text")}
          >
            <MenuItem value="html">HTML</MenuItem>
            <MenuItem value="text">Text</MenuItem>
          </Select>
        </FormControl>

        <TextField
          label="Log File Path"
          variant="outlined"
          value={filePath}
          onChange={(e) => setFilePath(e.target.value)}
          fullWidth
          margin="normal"
        />

        <Button
          onClick={handleStrategyChange}
          color="primary"
          variant="contained"
        >
          Change Strategy
        </Button>

        {message && (
          <Typography variant="body1" mt={2}>
            {message}
          </Typography>
        )}
      </Box>
    </Container>
  );
};

export default StrategyPage;
