import { concatBuffers } from "../concatBuffers.js"
import axios from 'axios'

export default class FileAccessService {

    async deleteFile(api, path) {
        const segments = path.substr(1).split("/")
        let url = api + "/file"
        for (const segment of segments) {
          url += "/" +  encodeURIComponent(segment)
        }        

        return axios.delete(url)
        .then((res) => {
          if (res.status !== 204) {
            throw(new Error("deleting playlist entry failed: HTTP " + res.statusText))
          }          
        })        
    }

    async fetchFileList(api, path) {
        return fetch(api + "/file" + path, {
            method: "get",
        })
        .then((res) => {
            if (!res.ok) {
                const error = new Error(res.statusText);
                throw error;
            }
            return res.json();
        })
        .then((data) => {
            return data;
        })
    }

    async getBackupList(api) {
        const fileList = await this.getDeepListCards(api)        
        let totalBytes = 0
        let totalFiles = 0
        for (const file of fileList) {
          totalFiles++
          totalBytes += file.size
        }
        return { fileList, totalBytes, totalFiles }
    }

    fileExistsInList(file, list) {
        for (const entry of list) {            
            if (entry.name === file.name) {                
                return true
            }
        }
        console.log("Adding " + file.name)
        return false
    }

    fileExistsInListSameSize(file, list) {
        for (const entry of list) {            
            if ((entry.name === file.name) && (entry.size === file.size)) {                
                return true
            } 
        }     
        console.log("Adding " + file.name + " because size differs")   
        return false
    }    

    async getSyncList(api1, api2) {
        const fileList1 = await this.getDeepList(api1)
        const fileList2 = await this.getDeepList(api2)
        const fileList = []

        for (const file of fileList1) {
            if (!this.fileExistsInList(file, fileList2)) {
                file.action = {
                    action: "copy",
                    cause: "new"
                }
                fileList.push(file)
            } else if (!this.fileExistsInListSameSize(file, fileList2)) {                
                file.action = {
                    action: "copy",
                    cause: "size"
                }
                fileList.push(file)
            }
        }
        for (const file of fileList2) {
            if (!this.fileExistsInList(file, fileList1)) {
                file.action = {
                    action: "delete"
                }
                fileList.push(file)
            }
        }
        let totalBytes = 0
        let totalFiles = 0
        for (const file of fileList) {
            totalFiles++
            if (file.action.action === "copy") {
                totalBytes += file.size      
            }
        }

        return { fileList, totalBytes, totalFiles }
    }

    async getDeepList(api) {    
        let fileList = []
        fileList = fileList.concat(await this.getDeepListSystem(api))
        fileList = fileList.concat(await this.getDeepListCards(api))
        console.log(fileList)
        return fileList
    }

    async getDeepListCards(api) {

        const cardsList = await this.fetchFileList(api, "/cards");
        const fileList = [];        
  
        for (const card of cardsList) {
          if (card.type === "directory") {
            const cardContents = await this.fetchFileList(api, card.name)
            for (const content of cardContents) {
              fileList.push({
                  name: content.name,
                  size: Number(content.size)
              })
            }
          }
        }

        return fileList
    }

    async getCardMetaData(api, cardId) {
        try {
            const metaContents = await fetch(api + "/file/cards/" + cardId + "/meta.txt", {
                method: "get",
            })
            if (metaContents.status !== 200) {
                throw(new Error("No meta.txt found for card " + cardId))
            }
            return metaContents.text()
        } catch (error) {
            console.log("No meta.txt found for card " + cardId)
            return ""
        }
        
    }

    async getListCards(api) {

        const cardsList = await this.fetchFileList(api, "/cards");
        const cardsListWithMeta = [];        
  
        for (const card of cardsList) {
            const cardId = card.name.split("/")[2]
            if (card.type === "directory") {
            const metaContents = await this.getCardMetaData(api, cardId)
            cardsListWithMeta.push({
                id: cardId,
                name: metaContents.replace(/(\r\n|\n|\r)/gm, ""),
                numtracks: 0
            })
          }
        }

        return cardsListWithMeta
    }

    async getDeepListSystem(api) {
        const sourceList = await this.fetchFileList(api, "/system");
        const fileList = [];        
        
        for (const file of sourceList) {
          if (file.type.startsWith("file")) {
              fileList.push({
                  name: file.name,
                  size: Number(file.size)
            })
          }
        }
        return fileList
    }



    // only one download at a time
    async fetchFileWithProgress(api, path, progress) {

        console.log("Fetching " + path)

        progress.currentFile.currentBytes = 0

        const response = await fetch(api + "/file" + path)
        const reader = response.body.getReader()

        const contentLengthHeader = response.headers.get("Content-Length")
        progress.currentFile.totalBytes = parseInt(contentLengthHeader, 10)

        const data = new Uint8Array()
        return this.readWithProgress(reader, data, progress)
    }

    async readWithProgress(reader, data, progress) {
        const { value, done } = await reader.read()
        if (done || progress.canceled) return data
        data = concatBuffers(data, value)
        progress.currentFile.currentBytes += value.length
        progress.overall.currentBytes += value.length
        return this.readWithProgress(reader, data, progress)
    }

}