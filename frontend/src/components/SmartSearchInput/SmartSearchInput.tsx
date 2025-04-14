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
  const options = useMemo(() => {
    const endsWithSpace = query.endsWith(" ") || false;
    const trimmedQuery = query.trimEnd();
    const parts = trimmedQuery.split(/\s+/); // split into tokens
    const lastPart = parts[parts.length - 1] || "";

    // return ["ana", "mere"];
    // Case 1: If last part is empty (user typed space at the end), suggest new keys
    if (endsWithSpace || trimmedQuery === "") {
      return Object.keys(suggestions).map((key) => `${key}:`);
    }

    // Case 2: User is typing a key (no colon yet)
    if (!lastPart.includes(":")) {
      return Object.keys(suggestions)
        .filter((key) => key.startsWith(lastPart))
        .map((key) => `${key}:`);
    }

    // Case 3: User is typing a value
    const [key, partialValue = ""] = lastPart.split(":");
    const values = suggestions[key] || [];
    return values
      .filter((v) => v.startsWith(partialValue))
      .map((v) => `${key}:${v}`);
  }, [query, suggestions]);

  useEffect(() => {
    const endsWithSpace = query.endsWith(" ") || false;
    const trimmedQuery = query.trimEnd();
    const parts = trimmedQuery.split(/\s+/);
    const lastPart = parts[parts.length - 1] || "";

    console.log("DEBUG ----");
    console.log("Query:", query);
    console.log("Ends with space:", endsWithSpace);
    console.log("Trimmed Query:", trimmedQuery);
    console.log("Parts:", parts);
    console.log("Last Part:", lastPart);
    console.log("Suggestions:", options);
    console.log("------------");
    console.log(options);
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
