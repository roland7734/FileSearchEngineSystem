export interface SearchResult {
  path: string;
  preview: string;
}

export interface SearchAggregates {
  file_type: Record<string, number>;
  modified_year: Record<string, number>;
  language: Record<string, number>;
  size: {
    small: number;
    large: number;
  };
}

export interface SearchResponse {
  corrected_query: string;
  count: number;
  results: SearchResult[];
  aggregates: SearchAggregates;
  widgets: string[];
  folders: string[];
  query_widgets: string[];
  monthly_counts?: { [month: string]: number };
}
