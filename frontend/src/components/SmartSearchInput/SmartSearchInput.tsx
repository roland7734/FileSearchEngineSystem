import React, { useMemo, useEffect } from "react";
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
  const fixedKeys = ["accesstime", "content", "mimetype", "path", "size"];

  const options = useMemo(() => {
    const endsWithSpace = query.endsWith(" ") || false;
    const trimmedQuery = query.trimEnd();
    const parts = trimmedQuery.split(/\s+/);
    const lastPart = parts[parts.length - 1] || "";

    if (endsWithSpace || trimmedQuery === "") {
      return fixedKeys.map((key) => `${key}:`);
    }

    if (!lastPart.includes(":")) {
      return fixedKeys
        .filter((key) => key.startsWith(lastPart))
        .map((key) => `${key}:`);
    }

    const [key, partialValue = ""] = lastPart.split(":");
    const values = suggestions[key] || [];
    return values
      .filter((v) => v.startsWith(partialValue))
      .map((v) => `${key}:${v}`);
  }, [query, suggestions]);

  const handleChange = (_: any, newValue: string | null) => {
    if (!newValue) return;

    const endsWithSpace = /\s+$/.test(query);
    const parts = query.trimEnd().split(/\s+/);

    if (endsWithSpace || parts.length === 0) {
      setQuery(query + newValue + " ");
    } else {
      parts[parts.length - 1] = newValue;
      setQuery(parts.join(" ") + " ");
    }
  };

  return (
    <Autocomplete
      freeSolo
      disableClearable
      autoHighlight
      openOnFocus
      filterOptions={(x) => x}
      options={options}
      inputValue={query}
      onInputChange={(_, value, reason) => {
        if (reason === "input") {
          setQuery(value);
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
