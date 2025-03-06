import unreal
import pandas as pd
import os
from typing import Dict, List, Tuple

ProjectDir = unreal.SystemLibrary.get_project_directory()
ExcelDir = ProjectDir + "Data/DataExcel"
CSVDir = ProjectDir + "Data/DataCSV"

PropertyRowIdentifier = "Property"
PropertyColumnIndex = 0
IndexColumnIndex = 1

UnrealTypeMapping = {
            'int': 'int32',
            'string': 'FString',
            'list:int': 'TArray<int32>',
            'list:string': 'TArray<FString>'
}

UnrealEditorAssetLib = unreal.EditorAssetLibrary()

def ProcessConvertExcel() -> bool:
    # 해당 경로 xlsx파일 찾기
    FileList = os.listdir(ExcelDir)
    ExcelFileList = []

    for File in FileList:
        if File.startswith("_"):
            continue

        if File.endswith(".xlsx"):
            ExcelFileList.append(File)

    LodeadExcelAndWorkSheetNameList = []
    
    for ExcelFileNameAndExtension in ExcelFileList:
        if(ExcelFileNameAndExtension.startswith("~")):
            continue

        unreal.log_error(f"{ExcelFileNameAndExtension} Convert Start")

        try:
            ExcelFilePath = ExcelDir + "/" + ExcelFileNameAndExtension
            LoadedExcel = pd.read_excel(ExcelFilePath, sheet_name=None, header=None)

        except Exception as e:
            unreal.log_error(f"엑셀 파일({ExcelFileNameAndExtension}) 읽기 실패: {str(e)}")
            return False

        # 엑셀의 워크시트 단위로 컨버팅 진행
        for SheetName, SheetData in LoadedExcel.items():
            if SheetData.empty:
                continue

            # SheetName 중복 체크
            for LoadedFileName, LoadedSheetName in LodeadExcelAndWorkSheetNameList:
                if LoadedSheetName == SheetName:
                    unreal.log_error(f"{LoadedFileName} - {LoadedSheetName} 과 {ExcelFileNameAndExtension} - {SheetName}이 중복됩니다. 워크시트 이름을 수정해주세요")
                    return False

            LodeadExcelAndWorkSheetNameList.append((ExcelFileNameAndExtension, SheetName.lower()))

            unreal.log_error(f"{SheetName} 워크시트 Convert Start")

            try:
                ConvertExcelToCSV(ExcelFileNameAndExtension, SheetName, SheetData)

                ConvertCSVToDataTable(SheetName)

            except Exception as e:
                unreal.log_error(f"처리 중 에러 발생 {ExcelFileNameAndExtension}: {str(e)}")
                return False
        
        unreal.log_error(f"{ExcelFileNameAndExtension} Convert End")

    return True

def ConvertExcelToCSV(ExcelFileNameAndExtension, SheetName, SheetData: pd.DataFrame):
    PropertyMask = SheetData.iloc[:, 0] == PropertyRowIdentifier
    if not PropertyMask.any():
        unreal.log_error(f"{ExcelFileNameAndExtension} 파일의 {SheetName} 워크시트에 {PropertyColumnIndex} 열에 Property 값이 없습니다. 수정해주세요")
        return

    PropertyRowIndex = int(SheetData[PropertyMask].index[0])
    TypeRowIndex = PropertyRowIndex + 1
    StartColIndex = int(PropertyColumnIndex + 1)
    PropertyRowDataList = SheetData.iloc[PropertyRowIndex, StartColIndex:]

    LoadPropertyList = []
    ExceptPropertyColumnIndexList = []
    UnrealTypeDict = {}

    for ColumnIndex, Property in enumerate(PropertyRowDataList):
        if pd.isna(Property):
            continue

        ExcelActualColumnIndex = ColumnIndex + StartColIndex

        # _로 시작하는 Property(변수) 제외
        if Property.startswith("_"):
            ExceptPropertyColumnIndexList.append(ExcelActualColumnIndex)
        else:
            LoadPropertyList.append(Property)

            Type = UnrealTypeMapping.get(str(SheetData.iloc[TypeRowIndex, ExcelActualColumnIndex]), None)
            if Type is None:
                unreal.log_error(f"{ExcelFileNameAndExtension} 파일의 {SheetName} 워크시트에 {ExcelActualColumnIndex} 열에 Type({str(SheetData.iloc[TypeRowIndex, ExcelActualColumnIndex])})이 int, string, list:int, list:string이 아닙니다. 수정해주세요")
                return
            
            UnrealTypeDict[Property] = Type
    
    CSVData = []
    CSVData.append(','.join(LoadPropertyList))
        
    StartRowIndex = TypeRowIndex + 1
    IndexList = []

    for RowIndex in range(StartRowIndex, len(SheetData)):
        IndexValue = SheetData.iloc[RowIndex, IndexColumnIndex]
        if pd.isna(IndexValue):
            unreal.log_error(f"{ExcelFileNameAndExtension} 파일의 {SheetName} 워크시트에 {RowIndex + 1} 행에 Index가 비어있습니다. 수정해주세요")
            return

        if IndexValue in IndexList:
            unreal.log_error(f"{ExcelFileNameAndExtension} 파일의 {SheetName} 워크시트에 {RowIndex + 1} 행에 Index({IndexValue})가 중복됩니다. 수정해주세요")
            return
        
        IndexList.append(IndexValue)
            
        RowData = []

        for ColIndex in range(StartColIndex, len(SheetData.columns)):
            if ColIndex in ExceptPropertyColumnIndexList:
                continue

            Value = SheetData.iloc[RowIndex, ColIndex]
            if pd.isna(Value):
                continue
            
            Type = UnrealTypeDict[SheetData.iloc[PropertyRowIndex, ColIndex]]
            
            if 'TArray' in Type and ';' in str(Value):
                TempValue = str(Value).split(';')

                if 'int32' in Type:
                    Value = f"({','.join(TempValue)})"
                elif 'FString' in Type:
                    # FString은 각 요소마다 ""로 감싸야 함
                    TempValue = [f'""{v}""' for v in TempValue]
                    Value = f"({','.join(TempValue)})"

                # TArray로 변환할 때 ""로 감싸야 함
                Value = f'"{Value}"'

            RowData.append(str(Value))
        
        CSVData.append(','.join(RowData))

    CSVFilePath = f"{CSVDir}/{SheetName}.csv"
    if os.path.exists(CSVFilePath):
       try:
           os.remove(CSVFilePath)
       except Exception as e:
           unreal.log_error(f"Failed to delete CSV file: {str(e)}")
           return
    
    with open(CSVFilePath, 'w', encoding='utf-8') as f:
        f.write('\n'.join(CSVData))

def ConvertCSVToDataTable(SheetName):
    try:
        DataTableDir = "/Game/ProjectN/DataTable"
        DataTablePath = DataTableDir + f"/{SheetName}"
        
        if UnrealEditorAssetLib.does_asset_exist(DataTablePath):
            UnrealEditorAssetLib.delete_asset(DataTablePath)

        DataTableStructHeaderPath = f"/Script/ProjectN.{SheetName}DataTable"
        DataTableStructObject = unreal.load_object(None, DataTableStructHeaderPath)
        if not DataTableStructObject:
            unreal.log_error(f"해당 데이터 테이블 구조체가 없습니다. 클라에게 요청해주세요 {DataTableStructHeaderPath}")
            return
        
        DatatableFactory = unreal.CSVImportFactory()
        DatatableFactory.automated_import_settings.import_row_struct = DataTableStructObject
        
        DataTableTask = unreal.AssetImportTask()
        DataTableTask.filename = f"{CSVDir}/{SheetName}.csv"
        DataTableTask.destination_path = DataTableDir
        DataTableTask.destination_name = f"{SheetName}"
        DataTableTask.automated = True
        DataTableTask.save = True
        DataTableTask.factory = DatatableFactory
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([DataTableTask])
        
    except Exception as e:
        unreal.log_error(f"Fatal error creating DataTable: {str(e)}")
        unreal.log_error(f"Error type: {type(e).__name__}")

def main():
    success = ProcessConvertExcel()
    
    if success:
        unreal.log_warning("Conversion completed successfully")
    else:
        unreal.log_warning("Conversion failed")

if __name__ == "__main__":
    main()