import { SearchSuggestions } from "../models/searchSuggestions";
import axiosInstance from "./axiosInstance";

export const getSearchSuggestions = async (): Promise<SearchSuggestions> => {
  try {
    const response = await axiosInstance.get<SearchSuggestions>("/suggestions");
    return response.data;
  } catch (error: any) {
    console.error("Error fetching suggestions:", error);

    const errorMessage =
      (
        (error.response?.data?.error || "") +
        " " +
        (error.response?.data?.details || "")
      ).trim() || "An error occurred while fetching suggestions.";

    throw new Error(errorMessage);
  }
};
