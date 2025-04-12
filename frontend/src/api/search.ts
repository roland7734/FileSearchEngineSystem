// api/searchFiles.ts
import axiosInstance from "./axiosInstance";

export const searchFiles = async (query: string) => {
  try {
    const response = await axiosInstance.get(
      `/search?query=${encodeURIComponent(query)}`
    );
    return response.data;
  } catch (error: any) {
    console.error("Error searching files:", error);
    throw new Error(
      error.response?.data?.message ||
        "An error occurred while searching files."
    );
  }
};
