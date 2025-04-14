import React, { useState, useEffect } from "react";
import { Autocomplete, TextField } from "@mui/material";
import { SearchSuggestions } from "../../models/searchSuggestions";

interface SmartSearchInputProps {
  suggestions: SearchSuggestions;
  query: string;
  setQuery: (q: string) => void;
}

const SmartSearchInput: React.FC<SmartSearchInputProps> = ({
  suggestions,
  query,
  setQuery,
}) => {
  const [options, setOptions] = useState<string[]>([]);
  const [inputValue, setInputValue] = useState("");

  useEffect(() => {
    const parts = query.trim().split(/\s+/);
    const lastPart = parts[parts.length - 1] || "";

    if (!lastPart.includes(":")) {
      const matchingKeys = Object.keys(suggestions).filter((key) =>
        key.startsWith(lastPart)
      );
      setOptions(matchingKeys.map((key) => `${key}:`));
    } else {
      const [key, partialValue = ""] = lastPart.split(":");
      const values = suggestions[key] || [];
      setOptions(
        values
          .filter((v) => v.startsWith(partialValue))
          .map((v) => `${key}:${v}`)
      );
    }

    setInputValue(lastPart);
  }, [query, suggestions]);

  const handleChange = (_: any, newValue: string | null) => {
    if (!newValue) return;

    const parts = query.trim().split(/\s+/);
    parts[parts.length - 1] = newValue;
    const updatedQuery = parts.join(" ");
    setQuery(updatedQuery + " ");
    setInputValue("");
  };

  return (
    <Autocomplete
      freeSolo
      disableClearable
      options={options}
      inputValue={inputValue}
      onInputChange={(_, value, reason) => {
        if (reason === "input") {
          const parts = query.trim().split(/\s+/);
          parts[parts.length - 1] = value;
          setQuery(parts.join(" "));
          setInputValue(value);
        }
      }}
      onChange={handleChange}
      renderInput={(params) => (
        <TextField
          {...params}
          label="Smart Search"
          variant="outlined"
          fullWidth
        />
      )}
    />
  );
};

export default SmartSearchInput;
