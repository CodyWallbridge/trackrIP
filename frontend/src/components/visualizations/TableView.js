import { useEffect, useState } from "react";
import { useValues } from "../../hooks/useValues";
import Box from "@mui/material/Box";
import IconButton from "@mui/material/IconButton";
import CircularProgress from "@mui/material/CircularProgress";
import Tooltip from "@mui/material/Tooltip";
import Typography from "@mui/material/Typography";
import Moment from "react-moment";
import ErrorIcon from "@mui/icons-material/Error";
import ArrowLeftIcon from "@mui/icons-material/ArrowLeft";
import ArrowRightIcon from "@mui/icons-material/ArrowRight";
import CenteredBox from "../CenteredBox";
import VisualizationMenuButton from "../VisualizationMenuButton";

const TableView = ({ visualizationType, visualization, metadata }) => {
  const limit = 8;
  const { fieldId, fieldName } = visualization;
  const { sort } = metadata;
  const [offset, setOffset] = useState(0);
  const [values, totalValues, loading, error] = useValues(
    fieldId,
    sort,
    offset,
    limit
  );

  const firstPage = Math.floor(offset / limit) + 1;
  const lastPage = Math.max(1, Math.ceil(totalValues / limit));

  useEffect(() => {
    setOffset(0);

    return () => {};
  }, [fieldId, sort]);

  const handleNextPage = () => {
    setOffset(offset + values.length);
  };

  const handlePreviousPage = () => {
    setOffset(offset - limit);
  };

  return (
    <>
      <Box
        sx={{
          display: "flex",
          flexDirection: "row",
          alignItems: "center",
          pb: 1.5,
          borderBottom: "1px solid #0000001f",
        }}
      >
        <Box sx={{ flexGrow: 1 }}>{fieldName}</Box>
        <Box>
          <VisualizationMenuButton
            visualizationType={visualizationType}
            visualization={visualization}
            metadata={metadata}
          />
        </Box>
      </Box>

      {error ? (
        <CenteredBox>
          <ErrorIcon sx={{ fontSize: 50, mb: 1.5 }} />
          <Typography variant="h7" sx={{ userSelect: "none", mb: 2 }}>
            {error}
          </Typography>
        </CenteredBox>
      ) : (
        <>
          <Box
            sx={{
              display: "flex",
              flexDirection: "column",
              flexGrow: 1,
              fontWeight: 400,
            }}
          >
            {loading ? (
              <CenteredBox>
                <CircularProgress />
              </CenteredBox>
            ) : (
              values.map((value) => (
                <Box
                  key={value.id}
                  sx={{
                    display: "flex",
                    flexDirection: "row",
                    py: 1.25,
                    borderBottom: "1px solid #0000001f",
                  }}
                >
                  <Box
                    sx={{
                      flexGrow: 1,
                      display: "flex",
                      alignItems: "center",
                    }}
                  >
                    {value.value}
                  </Box>

                  <Box sx={{ flexGrow: 1, textAlign: "right" }}>
                    <Tooltip title={value.createdAt}>
                      <Box>
                        <Moment format="MMM D, YYYY, HH:mm:ss">
                          {value.createdAt}
                        </Moment>
                      </Box>
                    </Tooltip>
                  </Box>
                </Box>
              ))
            )}
          </Box>

          <Box
            sx={{
              display: "flex",
              flexDirection: "row",
              alignItems: "center",
              mt: 1.5,
              userSelect: "none",
            }}
          >
            <Box sx={{ flexGrow: 1, color: "#00000085" }}>
              Page {firstPage} of {lastPage}
            </Box>
            <IconButton
              disabled={loading || offset - limit < 0}
              onClick={handlePreviousPage}
            >
              <ArrowLeftIcon />
            </IconButton>
            <IconButton
              disabled={loading || offset + limit >= totalValues}
              onClick={handleNextPage}
            >
              <ArrowRightIcon />
            </IconButton>
          </Box>
        </>
      )}
    </>
  );
};

export default TableView;