//
//  DismissSheetView.swift
//  BeeQ
//
//  Created by user224354 on 9/8/22.
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
