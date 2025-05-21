import React from "react";
import "./Aggregates.scss";
import { SearchAggregates } from "../../models/searchResult";

interface AggregatesProps {
  aggregates: SearchAggregates;
}

const Aggregates: React.FC<AggregatesProps> = ({ aggregates }) => {
  const formatAggregate = (data: Record<string, number>) =>
    Object.entries(data)
      .map(([key, value]) => `${key} (${value})`)
      .join("\n");

  const formatYears = (data: [number, number][]) =>
    data.map(([year, count]) => `${year} (${count})`).join("\n");

  const sizes = `Small (${aggregates.size.small})\nLarge (${aggregates.size.large})`;

  return (
    <div className="aggregates-wrapper">
      <table className="aggregates-table">
        <thead>
          <tr>
            <th>File Type</th>
            <th>Modified Year</th>
            <th>Language</th>
            <th>Size</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td style={{ whiteSpace: "pre-line", textAlign: "left" }}>
              {formatAggregate(aggregates.file_type)}
            </td>
            <td style={{ whiteSpace: "pre-line", textAlign: "left" }}>
              {formatYears(aggregates.modified_year as [number, number][])}
            </td>
            <td style={{ whiteSpace: "pre-line", textAlign: "left" }}>
              {formatAggregate(aggregates.language)}
            </td>
            <td style={{ whiteSpace: "pre-line", textAlign: "left" }}>
              {sizes}
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  );
};

export default Aggregates;
