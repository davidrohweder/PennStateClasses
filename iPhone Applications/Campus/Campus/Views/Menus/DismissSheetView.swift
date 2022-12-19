//
//  DismissSheetView.swift
//  Campus
//
//  Created by user224354 on 10/3/22.
//

import SwiftUI

struct DismissSheetView: View {
    @Environment (\.dismiss) private var dismiss
    var body: some View {
        Button("Dismiss") {
            dismiss()
        }
    }
}

struct DismissSheetView_Previews: PreviewProvider {
    static var previews: some View {
        DismissSheetView()
    }
}
